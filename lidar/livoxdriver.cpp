#include "livoxdriver.h"

using namespace livox;

//=======================================================================================
LivoxDriver::LivoxDriver( Config& config,
                          const BroabcastInfo& info,
                          QObject *parent )
    : QObject   ( parent        )
    , _conf     ( &config       )
    , _info     ( info          )
    , _seq_num  ( info.seq_num  )
    , _dev_type ( info.dev_type )
{
    _container = new LivoxContainer( config, _info.broadcast_code, this );

    _seq_num++;

    connect( this, &LivoxDriver::transmit_point_cloud,
             _container, &LivoxContainer::add_pack );

    connect( this, &LivoxDriver::transmit_info,
             _container, &LivoxContainer::set_info );

    connect( this, &LivoxDriver::transmit_imu,
             _container, &LivoxContainer::set_imu );

//    connect( _container, &LivoxContainer::transmit_packet_pnts,
//             this, &LivoxDriver::transmit_packet_pnts );

    _init_listen_ports();
    _set_handshake();
}
//=======================================================================================

//=======================================================================================
void LivoxDriver::_init_listen_ports()
{
    _sock_data = new QUdpSocket( this );
    _sock_cmd  = new QUdpSocket( this );

    if ( !_sock_data->bind() )
        throw verror << "Cannot bind to port: " << _sock_data->localPort();

    if ( !_sock_cmd->bind() )
        throw verror << "Cannot bind to port: " << _sock_cmd->localPort();

    connect( _sock_data, &QUdpSocket::readyRead, this, &LivoxDriver::_on_data );

    connect( _sock_cmd, &QUdpSocket::readyRead, this, &LivoxDriver::_on_command );
}
//=======================================================================================
void LivoxDriver::_on_command()
{
    while ( _sock_cmd->hasPendingDatagrams() )
    {
        auto dgram = _sock_cmd->receiveDatagram();

        vdeb << dgram.data().toHex(' ');

        auto address = dgram.senderAddress();
        if ( address != _info.address )
            throw verror << "The address: " << address << " is not for this Driver!";

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
        if ( ccrc32 != crc32 )
        {
            vwarning << "CRC32";
            continue;
        }

        auto view = buf.view();

        Frame<AckHandshake> head;
        auto ok = head.decode( &view );
        if (!ok)
        {
            vwarning << "Bad packet";
            continue;
        }

        if ( head.cmd_set == kCommandSetGeneral &&
             head.cmd_id  == kCommandIDGeneralHandshake &&
             !_was_lidar_init )
        {
            vdeb << "Handshake Successfull!";

            _heart_timer = new QTimer(this);
            _heart_timer->start( heartbeat_ms );
            connect( _heart_timer, &QTimer::timeout,
                     this, &LivoxDriver::_set_heartbeat );
        }
        else if ( head.cmd_set == kCommandSetGeneral &&
                  head.cmd_id  == kCommandIDGeneralHeartbeat &&
                  !_was_lidar_init )
            _init_lidar();
    }
}
//=======================================================================================
void LivoxDriver::_on_data()
{
    while ( _sock_data->hasPendingDatagrams() )
    {
        auto now = vsystem_time_point::now().microseconds().count();

        auto dgram = _sock_data->receiveDatagram();

        auto byte_data = dgram.data();
        vbyte_buffer_view view( byte_data.data(), uint( byte_data.size() ) );

        Package<LivoxSpherPoint> pack;
        pack.decode( &view );

        transmit_point_cloud( convert<Package<LivoxSpherPoint>>( pack ), now );
        transmit_info( pack.status_code, pack.timestamp );

        vdeb << "Receive Point Cloud Data from "
             << _info.broadcast_code
             << " of size" << dgram.data().size() << pack.cat();
    }
}
//=======================================================================================
void LivoxDriver::_init_lidar()
{
    _change_coord_system( PointDataType::kSpherical );

    _sampling( LidarSample::start );

    _set_mode( LidarMode::kLidarModeNormal );
    _set_weather_suppress( Turn::off );
    _set_extr_params();

    _was_lidar_init = true;
}
//=======================================================================================

//=======================================================================================
void LivoxDriver::_set_handshake()
{
    Frame<CmdHandshake> head;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeAck;

    head.data.user_ip   = _conf->ip().toIPv4Address();
    head.data.data_port = _sock_data->localPort();
    head.data.cmd_port  = _sock_cmd->localPort();
    head.data.imu_port  = _sock_data->localPort();

    auto dgram = head.encode();

    vdeb << dgram.to_Hex();

    auto sended = _sock_cmd->writeDatagram( dgram.data(),
                                            int( dgram.size() ),
                                            _info.address,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Send Handshake Request to" << _info.broadcast_code;
    else
        throw verror << "Cannot send Handshake" << _info.broadcast_code;
}
//=======================================================================================
void LivoxDriver::_set_heartbeat()
{
    Frame<CmdHeartbeat> head;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeCmd;

    auto dgram = head.encode();

    auto sended = _sock_cmd->writeDatagram( dgram.data(),
                                            int( dgram.size() ),
                                            _info.address,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Send Heartbeat Request to " << _info.broadcast_code;

    else
        verror << "Cannot send Heartbeat to" << _info.broadcast_code;
}
//=======================================================================================
void LivoxDriver::_sampling( const LidarSample sample )
{
    Frame<CmdSampling> head;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeCmd;

    head.data.sample_ctrl = sample;

    auto dgram = head.encode();

    auto sended = _sock_cmd->writeDatagram( dgram.data(),
                                            int( dgram.size() ),
                                            _info.address,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Set Lidar Sampling!";
    else
        throw verror << "Cannot set Lidar "
                     << _info.broadcast_code
                     << " Sampling((";
}
//=======================================================================================
void LivoxDriver::_change_coord_system( const PointDataType type )
{
    Frame<CmdCoordinateSystem> head;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeCmd;

    head.data.coordinate_type = type;

    auto dgram = head.encode();

    auto sended = _sock_cmd->writeDatagram( dgram.data(),
                                            int( dgram.size() ),
                                            _info.address,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Set Lidar" << _info.broadcast_code << "Coordinate System!";
    else
        throw verror << "Cannot set Lidar "
                     << _info.broadcast_code
                     << " Coordinate System((";
}
//=======================================================================================
void LivoxDriver::_set_mode( const LidarMode mode )
{
    Frame<CmdSetMode> head;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeCmd;

    head.data.lidar_mode = LidarMode::kLidarModeNormal;

    auto dgram = head.encode();

    auto sended = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            int( dgram.size() ),
                                            _info.address,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Set Lidar " << _info.broadcast_code << " Mode: " << mode;
    else
        throw verror << "Couldn't set Lidar " << _info.broadcast_code << " Mode((";
}
//=======================================================================================
void LivoxDriver::_set_extr_params()
{
    Frame<CmdWriteExtrinsicParams> head;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeCmd;

    {
        head.data.roll  = _conf->offset.roll;
        head.data.pitch = _conf->offset.pitch;
        head.data.yaw   = _conf->offset.yaw;
        head.data.x = _conf->offset.x;
        head.data.y = _conf->offset.y;
        head.data.z = _conf->offset.z;
    }

    auto dgram = head.encode();

    auto sended = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            int( dgram.size() ),
                                            _info.address,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Set Lidar " << _info.broadcast_code << " Extrinsic Parameters: ";
    else
        throw verror << "Couldn't set Lidar "
                     << _info.broadcast_code
                     << " Extrinsic Parameters((";
}
//=======================================================================================
void LivoxDriver::_set_weather_suppress( const Turn turn )
{
    Frame<CmdWeatherSuppression> head;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeCmd;

    head.data.state = turn;

    auto dgram = head.encode();

    auto sended = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            int( dgram.size() ),
                                            _info.address,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Set Lidar " << _info.broadcast_code << " Rain/Fog Suppression: " << turn;
    else
        throw verror << "Couldn't set Lidar "
                     << _info.broadcast_code
                     << " Rain/Fog Suppression((";
}
//=======================================================================================
void LivoxDriver::_set_return_mode( const PointCloudReturnMode mode )
{
    Frame<CmdSetReturnMode> head;
    head.seq_num  = _seq_num++;
    head.cmd_type = kCommandTypeCmd;

    head.data.mode = mode;

    auto dgram = head.encode();

    auto sended = _sock_cmd->writeDatagram( dgram.str().c_str(),
                                            int( dgram.size() ),
                                            _info.address,
                                            livox_port );
    if ( sended == int( dgram.size() ) )
        vdeb << "Set Lidar " << _info.broadcast_code
             << " Return Mode: " << mode;
    else
        throw verror << "Couldn't set Lidar "
                     << _info.broadcast_code
                     << " Rain/Fog Suppression((";
}
//=======================================================================================
