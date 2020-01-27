#include "lidar.h"

using namespace std;
using namespace livox;

//=======================================================================================

uint16_t Lidar::_seq_num = 0;

//=======================================================================================
Lidar::Lidar( QObject* parent )
    : QObject(parent)
{
    //-----------------------------------------------------------------------------------
    vbyte_buffer ip_addr;

    ip_addr.append( uchar(192) );
    ip_addr.append( uchar(168) );
    ip_addr.append( uchar(150) );
    ip_addr.append( uchar(90)  );
    _host_ip = QHostAddress( ip_addr.view().u32_BE() );
    //-----------------------------------------------------------------------------------
    _sock_listener = new QUdpSocket(this);
    if ( !_sock_listener->bind( host_bcast_port ) )
    {
        throw verror << "Cannot bind to port "
                     << host_data_port
                     << " (for listen livox broadcasts)";
    }

    connect( _sock_listener, &QUdpSocket::readyRead, this, &Lidar::_on_broadcast );
    //-----------------------------------------------------------------------------------
}
//=======================================================================================

//=======================================================================================
void Lidar::_on_broadcast()
{
    if ( _sock_data ) return;
    _init_listen_ports();

    if ( _sock_listener->hasPendingDatagrams() )
    {
        auto dgram = _sock_listener->receiveDatagram();

        vdeb << "Receive Broadcast";

        _livox_ip = dgram.senderAddress();
        auto port = dgram.senderPort();

        if ( port != livox_port )
            throw verror << "UDP Datagrama not from lidar Livox!";

        auto byte_data = dgram.data();
        vbyte_buffer_view view( byte_data.data(), uint(byte_data.size()) );

        Frame<BroabcastMessage> frame;
        auto ok = frame.decode( &view );

        auto ccrc = calc_crc32( byte_data.data(), uint(byte_data.size()) - 4 );
        //assert( ccrc == frame.crc_32 ); // Надо норм проверку.

        constexpr uint preambul_sz = sizeof( SdkPreamble );

        if ( !check_crc16( byte_data.data(), preambul_sz ) )
        {
            vwarning << "Bad preambul CRC16 in bcast message";
            return;
        }

        if ( frame.data.dev_type != kDeviceTypeLidarMid40 )
        {
            vwarning << "No Mid-40 connected device";
            return;
        }

        //        if ( frame.sof != kSdkProtocolSof )
        //            throw verror << "sof != kSdkProtocolSof!";

        if ( frame.data.cmd.cmd_set != kCommandSetGeneral )
            throw verror << "cmd_set != livox::kCommandSetGeneral";

        if ( frame.data.cmd.cmd_id != kCommandIDGeneralBroadcast )
            throw verror << "cmd_id != livox::kCommandIDGeneralBroadcast";

        vdeb << vcat( "Address: ", _livox_ip.toString(), " | ",
                      " Port: ", port, " | ",
                      frame.cat() );

        //_seq_num = frame.seq_num;
        //_seq_num++;
        _seq_num = 2;

        _send_handshake();
    }
}
//=======================================================================================
void Lidar::_init_listen_ports()
{
    _sock_data = new QUdpSocket(this);
    _sock_cmd  = new QUdpSocket(this);

    if ( !_sock_data->bind() )
        throw verror << "Cannot bind to port ";
//                     << host_data_port
//                     << " (for listen livox broadcasts)";

    if ( !_sock_cmd->bind() )
        throw verror << "Cannot bind to port ";
//                     << host_cmd_port
//                     << " (for listen livox broadcasts)";

    connect( _sock_data, &QUdpSocket::readyRead, this, &Lidar::_on_data );

    connect( _sock_cmd, &QUdpSocket::readyRead, this, &Lidar::_on_command );
}
//=======================================================================================

//=======================================================================================
void Lidar::_send_handshake()
{
    Cmd cmd( kCommandSetGeneral, kCommandIDGeneralHandshake );

    Head head;
    head.version  = kSdkVer0;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeAck;

    CmdHandshake req;
    req.user_ip   = _host_ip.toIPv4Address();
    req.data_port = _sock_data->localPort();
    req.cmd_port  = _sock_cmd->localPort();
    req.imu_port  = _sock_data->localPort();

    auto dgram = head.encode( req );

    auto sended = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            int(dgram.size()),
                                            _livox_ip,
                                            livox_port );
    if ( sended == int(dgram.size()) )
        vdeb << "Send Handshake Request";
    else
        throw verror << "Cannot send Handshake";
}
//=======================================================================================
void Lidar::_on_command()
{
    while ( _sock_cmd->hasPendingDatagrams() )
    {
        auto dgram = _sock_cmd->receiveDatagram();

        vdeb << "Receive Handshake Response";
        vdeb << dgram.data().toHex(' ');

        auto address = dgram.senderAddress();
        if (address != _livox_ip) continue;
        //        auto port = dgram.senderPort();

        auto byte_data = dgram.data();
        uint32_t crc32;
        vbyte_buffer buf;
        {
            vbyte_buffer b( dgram.data().toStdString() );
            auto v = b.view();
            buf = v.buffer( v.remained() - 4 );
            crc32 = v.u32_LE();
        }
        auto ccrc32 = calc_crc32( buf.str().c_str(), buf.size() );
        if (ccrc32 != crc32 )
        {
            vwarning << "CRC32";
            continue;
        }

        auto view = buf.view();

        Head head;
        auto ok = head.decode( &view );
        if (!ok)
        {
            vwarning << "Bad packet";
            continue;
        }
        //vdeb << head.str() << view.remained();

        //        else if ( packet.cmd_set == kCommandSetGeneral &&
        //                  packet.cmd_code == kCommandIDGeneralHandshake )

        if (head.cmd_set == kCommandSetGeneral &&
                head.cmd_id  == kCommandIDGeneralHandshake )
        {
            vdeb << "gotcha ack req";

            _heart_timer = new QTimer(this);
            _heart_timer->start( heartbeat_ms );
            connect( _heart_timer, &QTimer::timeout, this, &Lidar::_send_heartbeat );
        }


        //        Frame<AckHandshake> frame;
        //        frame.decode( &view );

        //        vdeb << vcat( "Address: ", address.toString(), " | ",
        //                      " Port: ", port, " | ",
        //                      frame.cat() );
    }
}
//=======================================================================================
void Lidar::_on_data()
{
    while ( _sock_data->hasPendingDatagrams() )
    {
        auto dgram = _sock_data->receiveDatagram();

        vdeb << "Receive Handshake Response";
        vdeb << dgram.data().toHex(' ');
        continue;
    }
}
//=======================================================================================

//=======================================================================================
void Lidar::_send_heartbeat()
{
    Head head;

    head.version  = kSdkVer0;
    head.cmd_type = kCommandTypeCmd;
    head.seq_num  = _seq_num++;

    auto dgram = head.encode( HeartBeat2{} );

    auto sended = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            int(dgram.size()),
                                            _livox_ip,
                                            livox_port );
    if ( sended == int(dgram.size()) )
        vdeb << "Send Heartbeat Request";
    else
        vwarning << "sended";

//    head.seq_num = _seq_num;
//    dgram = head.encode( DevInfo{} );
//    _sock_cmd->writeDatagram( dgram.str().c_str(),
//                              int(dgram.size()),
//                              _livox_ip,
//                              livox_port );
}
//=======================================================================================
