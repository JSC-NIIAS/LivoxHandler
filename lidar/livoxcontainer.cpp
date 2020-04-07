#include "livoxcontainer.h"

//=======================================================================================
LivoxContainer::LivoxContainer( const Config& conf,
                                const BroabcastInfo& info,
                                QObject* parent )
    : QObject ( parent )
    , _info   ( info   )
{
#ifdef GUI
    _scatter = new CustomScatter( 1000, _info.broadcast_code );
#endif

    _data_timer = new QTimer( this );
    _data_timer->start( conf.main_params.data_freequency );

    connect( _data_timer, &QTimer::timeout,
             [this]
    {
#ifdef GUI
        _scatter->plot_pnts( _packs );
#endif
        emit transmit_packet_pnts( _packs );

        _packs.clear();
    } );

    //-----------------------------------------------------------------------------------

    if ( _info.dev_type != kDeviceTypeLidarMid40 )
    {
        _imu_timer = new QTimer( this );
        _imu_timer->start( conf.main_params.imu_freequency );

        connect( _imu_timer, &QTimer::timeout,
                 [this]
        {
        } );
    }

    //-----------------------------------------------------------------------------------

    _info_timer = new QTimer( this );
    _info_timer->start( conf.main_params.info_freequency );

    connect( _info_timer, &QTimer::timeout,
             [&]
    {
    } );
}
//=======================================================================================


//=======================================================================================
void LivoxContainer::add_pack( const Pack& data, const int32_t ts )
{
    _packs.append( data );
    _capture_time = ts;
}
//=======================================================================================
void LivoxContainer::set_imu( const LivoxImuPoint& pnt )
{
    _imu_pnts.append( pnt );
}
//=======================================================================================
void LivoxContainer::set_info( const LidarErrorCode& code, const uint64_t ts )
{
    _sensor_info = code;
    _sensor_info.timestamp = ts;
}
//=======================================================================================
