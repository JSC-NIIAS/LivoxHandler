#include "lidar.h"

using namespace std;
using namespace livox;

//=======================================================================================

uint16_t Lidar::_seq_num = 0;

//=======================================================================================
Lidar::Lidar( QObject* parent )
    : QObject( parent )
{
    //-----------------------------------------------------------------------------------

    vbyte_buffer ip_addr;

    ip_addr.append( uchar(192) );
    ip_addr.append( uchar(168) );
    ip_addr.append( uchar(150) );
    ip_addr.append( uchar(78)  );
    _host_ip = QHostAddress( ip_addr.view().u32_BE() );

    //-----------------------------------------------------------------------------------

    _sock_listener = new QUdpSocket(this);

    if ( !_sock_listener->bind( host_bcast_port ) )
    {
        throw verror << "Cannot bind to port "
                     << host_data_port
                     << " (for listen livox broadcasts)";
    }

    connect( _sock_listener, &QUdpSocket::readyRead, this, &Lidar::_get_broadcast );

    //-----------------------------------------------------------------------------------

    _scatter = new CustomScatter( 1000 );

    _data_timer = new QTimer( this );
    _data_timer->start( data_ms );
    connect( _data_timer, &QTimer::timeout,
             [this]
    {
        _scatter->plot_pnts( _spherical_to_cartezian( _pnts ) );
        _pnts.clear();
    });

    //-----------------------------------------------------------------------------------
}
//=======================================================================================

//=======================================================================================
QList<LivoxRawPoint> Lidar::_spherical_to_cartezian( const QList<LivoxSpherPoint>& pnts )
{
    QList<LivoxRawPoint> res;

    for ( const auto& pnt: pnts )
    {
        LivoxRawPoint tmp;

        tmp.x = int32_t( pnt.depth * qSin( pnt.theta ) * qCos( pnt.phi ) );
        tmp.y = int32_t( pnt.depth * qSin( pnt.theta ) * qSin( pnt.phi ) );
        tmp.z = int32_t( pnt.depth * qCos( pnt.theta ) );
        tmp.reflectivity = pnt.reflectivity;

        res.push_back( tmp );
    }

    return res;
}
//=======================================================================================

//=======================================================================================
void Lidar::_get_broadcast()
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

        _set_handshake();
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
void Lidar::_init_lidar()
{
    _sampling( LidarSample::start );

    _change_coord_system( PointDataType::kSpherical );
//    _set_mode( LidarMode::kLidarModeNormal );
    _set_weather_suppress( Turn::off );
    _set_extr_params();

    _was_lidar_init = true;
}
//=======================================================================================

//=======================================================================================
void Lidar::_set_handshake()
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
            //            continue;
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

        if ( head.cmd_set == kCommandSetGeneral &&
             head.cmd_id  == kCommandIDGeneralHandshake )
        {
            vdeb << "gotcha ack req";

            _heart_timer = new QTimer(this);
            _heart_timer->start( heartbeat_ms );
            connect( _heart_timer, &QTimer::timeout, this, &Lidar::_set_heartbeat );
        }
        else if ( head.cmd_set == kCommandSetGeneral &&
                  head.cmd_id  == kCommandIDGeneralHeartbeat &&
                  !_was_lidar_init )
            _init_lidar();
    }
}
//=======================================================================================
void Lidar::_on_data()
{
    while ( _sock_data->hasPendingDatagrams() )
    {
        auto dgram = _sock_data->receiveDatagram();

        auto byte_data = dgram.data();
        vbyte_buffer_view view( byte_data.data(), uint( byte_data.size() ) );

        Package<LivoxSpherPoint> pack;
        pack.decode( &view );

        _pnts.append( pack.pnts );

        vdeb << "Receive Point Cloud Data of size" << dgram.data().size() << pack.cat();

        continue;
    }
}
//=======================================================================================

//=======================================================================================
void Lidar::_set_heartbeat()
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
    if ( sended == int( dgram.size() ) )
        vdeb << "Send Heartbeat Request";

    else
        vwarning << "sended";
}
//=======================================================================================

//=======================================================================================
void Lidar::_sampling( const LidarSample sample )
{
    Head head;
    head.version  = kSdkVer0;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeCmd;

    Sampling samp;
    samp.sample_ctrl = sample;

    auto dgram = head.encode( samp );

    vdeb << dgram.to_Hex();

    auto sended = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            int( dgram.size() ),
                                            _livox_ip,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Set Lidar Sampling!";
    else
        throw verror << "Cannot set Lidar Sampling((";
}
//=======================================================================================

//=======================================================================================
void Lidar::_change_coord_system( const PointDataType type )
{
    Head head;
    head.version  = kSdkVer0;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeCmd;

    CoordinateSystem system;
    system.coordinate_type = type;

    auto dgram = head.encode( system );

//    vdeb << dgram.to_Hex();

    auto sended = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            int( dgram.size() ),
                                            _livox_ip,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Set Lidar Coordinate System!";
    else
        throw verror << "Cannot set Lidar Coordinate System((";
}
//=======================================================================================


//=======================================================================================
void Lidar::_set_mode( const LidarMode mode )
{
    Head head;
    head.version  = kSdkVer0;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeCmd;

    Mode lidar;
    lidar.lidar_mode = LidarMode::kLidarModeNormal;

    auto dgram = head.encode( lidar );

//    vdeb << dgram.to_Hex();

    auto sended = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            int( dgram.size() ),
                                            _livox_ip,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Set Lidar Mode: " << mode;
    else
        throw verror << "Couldn't set Lidar Mode((";
}
//=======================================================================================
void Lidar::_set_extr_params()
{
    Head head;
    head.version  = kSdkVer0;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeCmd;

    LidarExtrinsicParameters eparams;
    {
        eparams.params.roll  = 0.0;
        eparams.params.pitch = 0.0;
        eparams.params.yaw   = 0.0;
        eparams.params.x = 0;
        eparams.params.y = 0;
        eparams.params.z = 450;
    }

    auto dgram = head.encode( eparams );

//    vdeb << dgram.to_Hex();

    auto sended = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            int( dgram.size() ),
                                            _livox_ip,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Set Lidar Extrinsic Parameters: ";
    else
        throw verror << "Couldn't set Lidar Extrinsic Parameters((";
}

void Lidar::_set_weather_suppress( const Turn turn )
{
    Head head;
    head.version  = kSdkVer0;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeCmd;

    LidarWeatherSuppress sup;
    sup.state = turn;

    auto dgram = head.encode( sup );

//    vdeb << dgram.to_Hex();

    auto sended = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            int( dgram.size() ),
                                            _livox_ip,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Set Lidar Rain/Fog Suppression: " << turn;
    else
        throw verror << "Couldn't set Lidar Rain/Fog Suppression((";
}
//=======================================================================================
