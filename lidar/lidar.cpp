#include "lidar.h"

using namespace std;
using namespace livox;

//=======================================================================================

uint16_t Lidar::_seq_num = 0;

//=======================================================================================
Lidar::Lidar( QObject* parent )
{
    _sock_data = new QUdpSocket( this );
    _sock_cmd = new QUdpSocket( this );
    _heart_timer = new QTimer( this );
    _heart_timer->start( heartbeat_ms );

    if ( !_sock_data->bind( host_data_port ) )
        throw verror << "Cannot bind to port "
                     << host_data_port
                     << " (for listen livox broadcasts)";

    if ( !_sock_cmd->bind( host_cmd_port ) )
        throw verror << "Cannot bind to port "
                     << host_cmd_port
                     << " (for listen livox broadcasts)";

    connect( _sock_data, &QUdpSocket::readyRead, this, &Lidar::_on_broadcast );
}
//=======================================================================================

//=======================================================================================
void Lidar::_on_broadcast()
{
    while ( _sock_data->hasPendingDatagrams() )
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

        vdeb << vcat( "Address: ", address.toString(), " | ",
                      " Port: ", port, " | ",
                      frame.cat() );
    }
}
//=======================================================================================
