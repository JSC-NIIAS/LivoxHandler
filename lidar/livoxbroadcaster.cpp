#include "livoxbroadcaster.h"

using namespace livox;

//=======================================================================================
LivoxBroadcaster::LivoxBroadcaster( Config& config,
                                    QObject *parent )
    : QObject   ( parent      )
    , _conf     ( &config     )
    , _local_ip ( config.ip() )
{
    _sock_listener = new QUdpSocket( this );

    if ( !_sock_listener->bind( local_bcast_port ) )
        throw verror << "Cannot bind to port "
                     << local_bcast_port
                     << " (for listen livox broadcasts)";

    connect( _sock_listener, &QUdpSocket::readyRead, this, &LivoxBroadcaster::_get );
}
//=======================================================================================

//=======================================================================================
void LivoxBroadcaster::_get()
{
    while ( _sock_listener->hasPendingDatagrams() )
    {
        auto dgram = _sock_listener->receiveDatagram();

        BroabcastInfo info;

        info.address = dgram.senderAddress();
        info.port = dgram.senderPort();

        if ( info.port != livox_port )
        {
            vwarning << "UDP Datagrama not from Lidar Livox!";
            return;
        }

        auto byte_data = dgram.data();
        vbyte_buffer_view view( byte_data.data(), uint(byte_data.size()) );

        Frame<BroabcastMessage> frame;
        auto ok = frame.decode( &view );

        if ( !ok )
        {
            vwarning << "Cannot decode UDP Datagrama!";
            return;
        }

        auto ccrc = calc_crc32( byte_data.data(), uint( byte_data.size() ) - 4 );

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

        if ( frame.data.cmd.cmd_set != kCommandSetGeneral )
        {
            vwarning << "cmd_set != livox::kCommandSetGeneral";
            return;
        }

        if ( frame.data.cmd.cmd_id != kCommandIDGeneralBroadcast )
        {
            vwarning << "cmd_id != livox::kCommandIDGeneralBroadcast";
            return;
        }

        info.broadcast_code = frame.data.broadcast_code.c_str();
        info.seq_num = frame.seq_num;

        vdeb << vcat( "Receive Broadcast",
                      "Address: ", info.address.toString(), " | ",
                      " Port: ", info.port, " | ",
                      frame.cat() );

        emit receive( info );
    }
}
//=======================================================================================
