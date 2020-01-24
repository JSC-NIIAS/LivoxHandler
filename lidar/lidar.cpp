#include "lidar.h"

using namespace std;
using namespace livox;

//=======================================================================================

uint16_t Lidar::_seq_num = 0;

//=======================================================================================
Lidar::Lidar( QObject* parent )
{
    _sock_data = new QUdpSocket;
    _sock_cmd = new QUdpSocket;
    _heart_timer = new QTimer;
    _heart_timer->start( heartbeat_ms );

    vbyte_buffer ip_addr;

    ip_addr.append( uchar(192) );
    ip_addr.append( uchar(168) );
    ip_addr.append( uchar(150) );
    ip_addr.append( uchar(78)  );

    _host_ip = new QHostAddress( uint32_t( ip_addr.view().u32_BE() ) );

    if ( !_sock_data->bind( host_data_port ) )
        throw verror << "Cannot bind to port "
                     << host_data_port
                     << " (for listen livox broadcasts)";

    if ( !_sock_cmd->bind( host_cmd_port ) )
        throw verror << "Cannot bind to port "
                     << host_cmd_port
                     << " (for listen livox broadcasts)";

    connect( _sock_data, &QUdpSocket::readyRead, this, &Lidar::_on_broadcast );

    connect( this, &Lidar::handshake, this, &Lidar::_to_handshake );

    connect( _sock_cmd, &QUdpSocket::readyRead, this, &Lidar::_on_handshake );

    connect( this,
             &Lidar::heartbeat,
             [&]
    {
        connect( _heart_timer, &QTimer::timeout, this, &Lidar::_to_heartbeat );
    } );
}
//=======================================================================================

//=======================================================================================
void Lidar::_on_broadcast()
{
    if ( _sock_data->hasPendingDatagrams() )
    {
        auto dgram = _sock_data->receiveDatagram();

        vdeb << "Receive Broadcast";

        auto address = dgram.senderAddress();
        auto port = dgram.senderPort();

        if ( port != livox_port )
            throw verror << "UDP Datagrama not from lidar Livox!";

        auto byte_data = dgram.data();
        vbyte_buffer_view view( byte_data.data(), uint( byte_data.size() ) );

        Frame<BroabcastMessage> frame;
        frame.decode( &view );

        constexpr uint preambul_sz = sizeof( SdkPreamble );

        if ( !check_crc16( byte_data.data(), preambul_sz ) )
        {
            vwarning << "Bad preambul CRC16 in bcast message";
            return;
        }

        if ( frame.cmd.dev_type != kDeviceTypeLidarMid40 )
        {
            vwarning << "No Mid-40 connected device";
            return;
        }

        if ( frame.sof != kSdkProtocolSof )
            throw verror << "sof != kSdkProtocolSof!";

        if ( frame.cmd.cmd.cmd_set != kCommandSetGeneral )
            throw verror << "cmd_set != livox::kCommandSetGeneral";

        if ( frame.cmd.cmd.cmd_id != kCommandIDGeneralBroadcast )
            throw verror << "cmd_id != livox::kCommandIDGeneralBroadcast";

        vdeb << vcat( "Address: ", address.toString(), " | ",
                      " Port: ", port, " | ",
                      frame.cat() );

        _seq_num = frame.seq_num;
        _seq_num++;

        if ( !_livox_ip )
            _livox_ip = &address;

        emit handshake();
    }
}
//=======================================================================================

//=======================================================================================
void Lidar::_on_handshake()
{
    if ( _sock_cmd->hasPendingDatagrams() )
    {
        auto dgram = _sock_data->receiveDatagram();

        vdeb << "Receive Handshake Response";

        auto address = dgram.senderAddress();
        auto port = dgram.senderPort();

        auto byte_data = dgram.data();
        vbyte_buffer_view view( byte_data.data(), uint( byte_data.size() ) );

        Frame<AckHandshake> frame;
        frame.decode( &view );

        vdeb << vcat( "Address: ", address.toString(), " | ",
                      " Port: ", port, " | ",
                      frame.cat() );

        emit heartbeat();
    }
}
//=======================================================================================
void Lidar::_to_handshake()
{
    Cmd cmd( kCommandSetGeneral, kCommandIDGeneralHandshake );

    CmdHandshake cmd_hand( cmd,
                           _host_ip->toIPv4Address(),
                           host_data_port,
                           host_cmd_port,
                           host_imu_port );

    Frame<CmdHandshake> frame( kSdkProtocolSof,
                               kSdkVer0,
                               cmd_hand.size(),
                               kCommandTypeCmd,
                               _seq_num,
                               cmd_hand );

    auto dgram = frame.encode();

    auto status = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            dgram.size(),
                                            *_host_ip,
                                            host_cmd_port );
    if ( status )
        vdeb << "Send Handshake Request";
}
//=======================================================================================

//=======================================================================================
void Lidar::_to_heartbeat()
{
    Cmd cmd( kCommandSetGeneral, kCommandIDGeneralHeartbeat );

    CmdHeartbeat cmd_heart( cmd );

    Frame<CmdHeartbeat> frame( kSdkProtocolSof,
                               kSdkVer0,
                               cmd_heart.size(),
                               kCommandTypeCmd,
                               _seq_num,
                               cmd_heart );

    auto dgram = frame.encode();

    auto status = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            dgram.size(),
                                            *_host_ip,
                                            host_cmd_port );
    if ( status )
        vdeb << "Send Heartbeat Request";
}
//=======================================================================================
