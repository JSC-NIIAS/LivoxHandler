#include "livoxcontainer.h"

using namespace zcm;

//=======================================================================================
LivoxContainer::LivoxContainer( const Config& conf,
                                const QString& broadcast,
                                QObject* parent )
    : QObject ( parent )
{
    _pub = new ZcmPublish( conf );

#ifdef GUI
    _scatter = new CustomScatter( 1000, broadcast );
#endif

    _data_timer = new QTimer( this );
    _data_timer->start( conf.main_params.data_freequency );

    _imu_timer = new QTimer( this );
    _imu_timer->start( conf.main_params.imu_freequency );

    _info_timer = new QTimer( this );
    _info_timer->start( conf.main_params.info_freequency );

    connect( _data_timer, &QTimer::timeout,
             [this]
    {
#ifdef GUI
        _scatter->plot_pnts( _packs );
#endif
        emit transmit_packet_pnts( _packs );

        _pub->send_point_cloud( _packs, _capture_time );

        _packs.clear();
    } );

    connect( _imu_timer, &QTimer::timeout,
             [this]
    {
        _pub->send_imu_data();
    } );

    connect( _info_timer, &QTimer::timeout,
             [&]
    {
        _pub->send_info( _sensor_info, broadcast );
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
