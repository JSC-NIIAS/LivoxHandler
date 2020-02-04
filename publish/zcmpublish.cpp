#include "zcmpublish.h"

using namespace std;

//=======================================================================================
ZcmPublish::ZcmPublish( const Config& conf, const QString channel )
    : _conf    ( conf                               )
    , _zcm     ( conf.zcm_send.target.toStdString() )
    , _channel ( channel                            )
{

}
//=======================================================================================
ZcmPublish::~ZcmPublish()
{
    _zcm.flush();
}
//=======================================================================================


//=======================================================================================
void ZcmPublish::send_point_cloud( const QList<Pack>& data, const int32_t capture )
{
    if ( data.empty() )
        return;

    ZcmLivoxScan msg;

    msg.version = int8_t( data.last().version );
    msg.slot_id = int8_t( data.last().slot_id );
    msg.lidar_id = int8_t( data.last().lidar_id );
    msg.timestamp_type = int8_t( data.last().timestamp_type );
    msg.data_type = int8_t( data.last().data_type );

    auto min_ts = int64_t( data.first().timestamp );
    auto ts = min_ts;

    for ( const auto& pack: data )
        for ( const auto& pnt: pack.pnts )
        {
            if ( std::isinf( pnt.x ) || std::isnan( pnt.x ) ||
                 std::isinf( pnt.y ) || std::isnan( pnt.y ) ||
                 std::isinf( pnt.z ) || std::isnan( pnt.z ) )
                continue;

            ZcmLivoxPoint zcm_pnt;

            zcm_pnt.x = pnt.x;
            zcm_pnt.y = pnt.y;
            zcm_pnt.z = pnt.z;

            zcm_pnt.reflectivity = int8_t( pnt.reflectivity );

            {
                PointTag tag;

                tag.ret_num = 0;
                tag.intensity = 0;
                tag.position = 0;

                zcm_pnt.tag = tag;
            }

            zcm_pnt.timestamp = ts;

            msg.points.push_back( zcm_pnt );

            ts += delta_ns;
        }

    msg.points_count = int32_t( msg.points.size() );

    msg.service.u_timestamp = min_ts / 1000;

    auto now = vsystem_time_point::now().microseconds().count();

    msg.service.processing_time = int32_t( now - capture );

    _zcm.publish( _conf.zcm_send.data_ch().toStdString(), &msg );
}
//=======================================================================================
void ZcmPublish::send_imu_data()
{
    ZcmLivoxIMU msg;

//    _zcm->publish( _conf.zcm_send.imu_ch().toStdString(), &msg );
}
//=======================================================================================
void ZcmPublish::send_info(const LidarStatus& status , const QString broadcast)
{
    ZcmDeviceInfo msg;

    msg.temp_status =      status.temp_status;
    msg.volt_status =      status.volt_status;
    msg.motor_status =     status.motor_status;
    msg.dirty_warn =       status.dirty_warn;
    msg.firmware_err =     status.firmware_err;
    msg.pps_status =       status.pps_status;
    msg.device_status =    status.device_status;
    msg.fan_status =       status.fan_status;
    msg.self_heating =     status.self_heating;
    msg.ptp_status =       status.ptp_status;
    msg.time_sync_status = status.time_sync_status;
    msg.system_status =    status.system_status;

    msg.timestamp = int64_t( status.timestamp );

    _zcm.publish( QString( _conf.zcm_send.info_ch() + broadcast ).toStdString() , &msg );
}
//=======================================================================================
