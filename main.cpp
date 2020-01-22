#include "vlivox_hunter.h"

#include "vlog.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"

#include <QApplication>
#include <QUdpSocket>
#include <QNetworkDatagram>

#include <livox_sdk.h>

using namespace std;
using namespace livox;

//=======================================================================================

vbyte_buffer default_buffer()
{
    vbyte_buffer res;

    uint8_t sof = kSdkProtocolSof;
    uint8_t version = livox::kSdkVer0;
    uint16_t length = sizeof( HandshakeRequest ) +
                      sizeof( livox::SdkPacket ) - 1 + kSdkPacketCrcSize;
    uint8_t packet_type = livox::kCommandTypeCmd;
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
    vbyte_buffer res = default_buffer();

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
    vbyte_buffer res = default_buffer();

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
    ip_addr.append( uchar(240)  );

    vdeb.hex() << ip_addr.view().u32_BE();
    //    return 0;

    QApplication qapp( argc, argv );

    //---------------------------------------------------------------

    VLivox_Hunter hunter( "0TFDFG700602211", "195.168.150.240" );

    QUdpSocket *cmd = nullptr, *data = nullptr, *sens = nullptr;

    QObject::connect( &hunter,
                      &VLivox_Hunter::receive,
                      [&]( const Info& info )
    {
        vdeb << info.str();

        data = new QUdpSocket;
        Q_ASSERT( data->bind() );
        cmd = new QUdpSocket;
        Q_ASSERT( cmd->bind() );
        sens = new QUdpSocket;
        Q_ASSERT( sens->bind() );

        auto data_port = data->localPort();
        auto cmd_port = cmd->localPort();
        auto sens_port = sens->localPort();

        // Заполнение request пакета
        auto pk = build_handshake_pk( 0xc0a896f0,
                                      data_port,
                                      cmd_port,
                                      sens_port );

        vdeb << "Handshake buffer: " << pk.to_Hex();

        cmd->writeDatagram( pk.str().c_str(),
                            pk.str().size(),
                            info.address,
                            cmd_port ); // Вот здесь возможно неправильный порт указан, хотя какая разница какой, главное не 55000 или 65000

        // Реакция на получение ACK от сенсора
        QObject::connect( data,
                          &QUdpSocket::readyRead,
                          [&]()
        {
            vdeb << "gotcha!";
        } );

        QObject::connect( cmd,
                          &QUdpSocket::readyRead,
                          [&]()
        {
            vdeb << "gotcha!" << cmd_port;
            auto dgram = cmd->receiveDatagram();
            vdeb << dgram.data().toHex(' ');
        } );
    });

    return qapp.exec();

//    vdeb << build_handshake_pk( 0xc0a896f0, 56001, 55501, 56001 ).to_Hex('_');
}
//=======================================================================================
