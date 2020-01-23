#include "vlivox_hunter.h"

#include "vlog.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"

#include <QApplication>
#include <QUdpSocket>
#include <QNetworkDatagram>

#include <livox_sdk.h>

#include <QNetworkInterface>

using namespace std;
using namespace livox;

//=======================================================================================

vbyte_buffer default_buffer( const uint16_t length )
{
    vbyte_buffer res;

    uint8_t sof = kSdkProtocolSof;
    uint8_t version = livox::kSdkVer0;
    uint8_t packet_type = livox::kCommandTypeAck;
    uint16_t seq_num = 2;

    res.append( sof );
    res.append( version );
    res.append_LE( length );
    res.append( packet_type );
    res.append_LE( seq_num );
    uint16_t preamble_crc = calc_crc16( res.str().c_str(), uint(res.str().size()) );
    res.append_LE( preamble_crc );

    return res;
}

//=======================================================================================

vbyte_buffer build_handshake_pk( uint32_t ip,
                                 uint16_t data_port,
                                 uint16_t cmd_port,
                                 uint16_t sens_port )
{
    vbyte_buffer res = default_buffer( sizeof( HandshakeRequest ) +
                                       sizeof( livox::SdkPacket ) - 1 + kSdkPacketCrcSize );

    uint8_t cmd_set = livox::kCommandSetGeneral;
    uint8_t cmd_id = livox::kCommandIDGeneralHandshake;

    res.append( cmd_set );
    res.append( cmd_id );
    res.append_BE( ip );
    res.append_LE( data_port );
    res.append_LE( cmd_port  );
    res.append_LE( sens_port );

    uint32_t crc = calc_crc32( res.str().c_str(), res.str().size() );
    res.append_LE( crc );

    return res;
}

//=======================================================================================

vbyte_buffer build_heartbeat_pk( uint32_t ip,
                                 uint16_t data_port,
                                 uint16_t cmd_port,
                                 uint16_t sens_port )
{
    vbyte_buffer res = default_buffer( sizeof( livox::SdkPacket ) - 1 + kSdkPacketCrcSize );

    uint8_t cmd_set = livox::kCommandSetGeneral;
    uint8_t cmd_id = livox::kCommandIDGeneralHeartbeat;

    res.append( cmd_set );
    res.append( cmd_id );
    res.append_BE( ip );
    res.append_LE( data_port );
    res.append_LE( cmd_port  );
    res.append_LE( sens_port );

    uint32_t crc = calc_crc32( res.str().c_str(), res.str().size() );
    res.append_LE( crc );

    return res;
}

//=======================================================================================

int main( int argc, char **argv )
{
    vbyte_buffer ip_addr;

    ip_addr.append( uchar(192) );
    ip_addr.append( uchar(168) );
    ip_addr.append( uchar(150) );
    ip_addr.append( uchar(78)  );

    vdeb.hex() << ip_addr.view().u32_BE();
    //    return 0;

    QApplication qapp( argc, argv );

    //---------------------------------------------------------------

    VLivox_Hunter hunter( "0TFDFG700602211", "195.168.150.240" );

    QUdpSocket *cmd = nullptr, *data = nullptr, *sens = nullptr;

    auto local_ip = QHostAddress( uint32_t( ip_addr.view().u32_BE() ) );

    data = new QUdpSocket;
    Q_ASSERT( data->bind() );
    cmd = new QUdpSocket;
    Q_ASSERT( cmd->bind() );
    sens = new QUdpSocket;
    Q_ASSERT( sens->bind() );

    auto data_port = data->localPort();
    auto cmd_port = cmd->localPort();
    auto sens_port = sens->localPort();

    QTimer heart_timer;

    //-----------------------------------------------------------------------------------

    QObject::connect( &hunter,
                      &VLivox_Hunter::msg,
                      [&]( const Info& info )
    {
        vdeb << info.str();

        // Заполнение request пакета
        auto pk = build_handshake_pk( local_ip.toIPv4Address(),
                                      data_port,
                                      cmd_port,
                                      sens_port );

        cmd->writeDatagram( pk.str().c_str(),
                            pk.size(),
                            local_ip,
                            local_data_port ); //data_port ); // Вот здесь возможно неправильный порт указан, хотя какая разница какой, главное не 55000 или 65000

        /*
        // Реакция на получение ACK от сенсора
        QObject::connect( data,
                          &QUdpSocket::readyRead,
                          [&]()
        {
            if ( data->hasPendingDatagrams() )
            {
                auto dgram = data->receiveDatagram();

                Info res;

                res.address = local_ip;

                res.port = data_port;

                auto ba_data = dgram.data();
                vbyte_buffer_view view( ba_data.data(), uint( ba_data.size() ) );

                res.sdk.sof = view.u8();
                res.sdk.version = view.u8();
                res.sdk.length = view.u16_LE();
                res.sdk.packet_type = view.u8();
                res.sdk.seq_num = view.u16_LE();
                res.sdk.preamble_crc = view.u16_LE();
                (void) res.sdk.preamble_crc;
                res.sdk.cmd_set = view.u8();
                res.sdk.cmd_id = view.u8();

                //            res.broadcast_code = view.string( kBroadcastCodeSize );
                //            res.broadcast_code.pop_back(); // kill zero.
                res.dev_type = view.u8();
                view.omit(2);
                auto crc = view.u32_LE();

                //            assert( view.finished() );

                vdeb << "Handshake successful!";

                vdeb << res.str();

                data->disconnect();
                heart_timer.start( 800 );
                hunter.emit_heartbeat();
                hunter.disconnect();
                hunter.~VLivox_Hunter();
            }
        } );
        */
    });

    //-----------------------------------------------------------------------------------

    QObject::connect( &hunter,&VLivox_Hunter::heartbeat,
                      [&]()
    {
        QObject::connect( &heart_timer,
                          &QTimer::timeout,
                          [&]()
        {
            vdeb << "Send heartbeat.";

            auto pk = build_heartbeat_pk( local_ip.toIPv4Address(),
                                          data_port,
                                          cmd_port,
                                          sens_port );

            cmd->writeDatagram( pk.str().c_str(),
                                pk.size(),
                                local_ip,
                                local_data_port );
        } );

        QObject::connect( data,
                          &QUdpSocket::readyRead,
                          [&]()
        {
            if ( data->hasPendingDatagrams() )
            {
                auto dgram = data->receiveDatagram();

                Info res;

                res.address = local_ip;

                res.port = data_port;

                auto ba_data = dgram.data();
                vbyte_buffer_view view( ba_data.data(), uint( ba_data.size() ) );

                res.sdk.sof = view.u8();
                res.sdk.version = view.u8();
                res.sdk.length = view.u16_LE();
                res.sdk.packet_type = view.u8();
                res.sdk.seq_num = view.u16_LE();
                res.sdk.preamble_crc = view.u16_LE();
                (void) res.sdk.preamble_crc;
                res.sdk.cmd_set = view.u8();
                res.sdk.cmd_id = view.u8();

//            res.broadcast_code = view.string( kBroadcastCodeSize );
//            res.broadcast_code.pop_back(); // kill zero.

                res.dev_type = view.u8();
                view.omit(2);
                auto crc = view.u32_LE();

                //            assert( view.finished() );

                vdeb << "Heartbeat successful!";

                vdeb << res.str();
            }
        } );
    } );

    //-----------------------------------------------------------------------------------

    return qapp.exec();
}
//=======================================================================================
