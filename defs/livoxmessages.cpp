#include "livoxmessages.h"

using namespace std;
using namespace livox;

//=======================================================================================
bool Package::decode( vbyte_buffer_view* view )
{
    version = view->u8();

    if ( version != 5 )
        throw verror << "Packet Protocol Version != 5";

    slot_id = view->u8();
    lidar_id = view->u8();

    if ( lidar_id != 1 )
        throw verror << "Point Cloud not from Mid-40 lidar.";

    reserved = view->u8();

    StatusData sd;
    status_code = sd.fill( view->u32_LE() );

    timestamp_type = view->u8();
    data_type = view->u8();
    timestamp = view->u64_LE();

    auto ost = view->remained() % sizeof ( LivoxSpherPoint );

    while ( !view->finished() && ( ost == 0 ) )
    {
        LivoxSpherPoint pnt;

        pnt.depth = view->u32_LE();
        pnt.theta = view->u16_LE();
        pnt.phi   = view->u16_LE();
        pnt.reflectivity = view->u8();

        pnts.push_back( pnt );
    }

    return true;
}
//=======================================================================================
string Package::cat()
{
    return vcat( " | version: ",         int( version ),
                 " | slot_id: ",         int( slot_id ),
                 " | lidar_id: ",        int( lidar_id ),
                 " | status_code: ",     int( status_code.system_status ),
                 " | timestamp_type: ",  int( timestamp_type ),
                 " | data_type: ",       int( data_type ),
                 " | timestamp: ",       std::chrono::nanoseconds( timestamp )
                 );

}
//=======================================================================================


//=======================================================================================
LidarErrorCode StatusData::fill( const uint32_t status )
{
    LidarErrorCode res;

    _status = status;

    res.temp_status      = err.temp_status;
    res.volt_status      = err.volt_status;
    res.motor_status     = err.motor_status;
    res.dirty_warn       = err.dirty_warn;
    res.firmware_err     = err.firmware_err;
    res.pps_status       = err.pps_status;
    res.device_status    = err.device_status;
    res.fan_status       = err.fan_status;
    res.self_heating     = err.self_heating;
    res.ptp_status       = err.ptp_status;
    res.time_sync_status = err.time_sync_status;
    res.rsvd             = err.rsvd;
    res.system_status    = err.system_status;

    return res;
}
//=======================================================================================
