#include "generalmessages.h"

//=======================================================================================
uint16_t MsgBroadcast::length()
{
    return 16 + sizeof ( dev_type ) + sizeof ( reserved );
}
//=======================================================================================
void MsgBroadcast::decode( vbyte_buffer_view* view )
{
    broadcast_code = view->string( kBroadcastCodeSize );
    broadcast_code.pop_back(); // kill zero.
    dev_type = view->u8();
    reserved = view->u16_LE();
}
//=======================================================================================
vbyte_buffer MsgBroadcast::encode()
{
    vbyte_buffer buf;

    buf.append( broadcast_code );
    buf.append( dev_type );
    buf.append_LE( reserved );

    return buf;
}
//=======================================================================================
std::string MsgBroadcast::cat()
{
    return vcat( " | broadcast_code: ", broadcast_code,
                 " | dev_type: ", int( dev_type ),
                 " | reserved: ", int(reserved) );
}
//=======================================================================================


//=======================================================================================
uint16_t CmdHandshake::length()
{
    return sizeof ( uint32_t ) + 3 * sizeof ( uint16_t );
}
//=======================================================================================
void CmdHandshake::decode( vbyte_buffer_view* view )
{
    user_ip   = view->u32_LE();
    data_port = view->u16_LE();
    cmd_port  = view->u16_LE();
    imu_port  = view->u16_LE();
}
//=======================================================================================
vbyte_buffer CmdHandshake::encode()
{
    vbyte_buffer buf;

    buf.append_BE( user_ip   );
    buf.append_LE( data_port );
    buf.append_LE( cmd_port  );
    buf.append_LE( imu_port  );

    return buf;
}
//=======================================================================================
std::string CmdHandshake::cat()
{
    return vcat( " | user_ip: ", int( user_ip ),
                 " | data_port: ", int( data_port ),
                 " | cmd_port: ", int( cmd_port ),
                 " | imu_port: ", int( imu_port ));
}
//=======================================================================================


//=======================================================================================
uint16_t AckHandshake::length()
{
    return sizeof ( ret_code );
}
//=======================================================================================
void AckHandshake::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckHandshake::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckHandshake::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) );
}
//=======================================================================================


//=======================================================================================
uint16_t CmdQueryDeviceInfo::length()
{
    return 0;
}
//=======================================================================================
void CmdQueryDeviceInfo::decode( vbyte_buffer_view* view )
{

}
//=======================================================================================
vbyte_buffer CmdQueryDeviceInfo::encode()
{
    return vbyte_buffer();
}
//=======================================================================================
std::string CmdQueryDeviceInfo::cat()
{
    return vcat();
}
//=======================================================================================


//=======================================================================================
uint16_t AckQueryDeviceInfo::length()
{
    return sizeof ( ret_code ) + sizeof ( version );
}
//=======================================================================================
void AckQueryDeviceInfo::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
    version = view->u32_LE();
}
//=======================================================================================
vbyte_buffer AckQueryDeviceInfo::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );
    buf.append_LE( version );

    return buf;
}
//=======================================================================================
std::string AckQueryDeviceInfo::cat()
{
    return vcat( " | ret_code: ", int( ret_code ),
                 " | version: ", int( version ) );
}
//=======================================================================================


//=======================================================================================
uint16_t CmdHeartbeat::length()
{
    return 0;
}
//=======================================================================================
void CmdHeartbeat::decode( vbyte_buffer_view* view )
{

}
//=======================================================================================
vbyte_buffer CmdHeartbeat::encode()
{
    return vbyte_buffer();
}
//=======================================================================================
std::string CmdHeartbeat::cat()
{
    return vcat();
}
//=======================================================================================


//=======================================================================================
uint16_t AckHeartbeat::length()
{
    return 3 * sizeof ( uint8_t ) + sizeof ( uint32_t );
}
//=======================================================================================
void AckHeartbeat::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
    work_state = view->u8();
    feature_msg = view->u8();
    ack_msg = view->u32_LE();
}
//=======================================================================================
vbyte_buffer AckHeartbeat::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );
    buf.append( work_state );
    buf.append( feature_msg );
    buf.append_LE( ack_msg );

    return buf;
}
//=======================================================================================
std::string AckHeartbeat::cat()
{
    return vcat( " | ret_code: ", int( ret_code ),
                 " | work_state: ", int( work_state ),
                 " | feature_msg: ", int( feature_msg ),
                 " | ack_msg: ", int( ack_msg ) );
}
//=======================================================================================


//=======================================================================================
uint16_t CmdSampling::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void CmdSampling::decode( vbyte_buffer_view* view )
{
    sample_ctrl = view->u8();
}
//=======================================================================================
vbyte_buffer CmdSampling::encode()
{
    vbyte_buffer buf;

    buf.append( sample_ctrl );

    return buf;
}
//=======================================================================================
std::string CmdSampling::cat()
{
    return vcat( " | sample_ctrl: ", int( sample_ctrl ) );
}
//=======================================================================================


//=======================================================================================
uint16_t AckSampling::length()
{
    return sizeof ( uint8_t  );
}
//=======================================================================================
void AckSampling::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckSampling::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckSampling::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) );
}
//=======================================================================================


//=======================================================================================
uint16_t CmdCoordinateSystem::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void CmdCoordinateSystem::decode( vbyte_buffer_view* view )
{
    coordinate_type = view->u8();
}
//=======================================================================================
vbyte_buffer CmdCoordinateSystem::encode()
{
    vbyte_buffer buf;

    buf.append( coordinate_type );

    return buf;
}
//=======================================================================================
std::string CmdCoordinateSystem::cat()
{
    return vcat( " | coordinate_type: ", int( coordinate_type ) );
}
//=======================================================================================


//=======================================================================================
uint16_t AckCoordinateSystem::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void AckCoordinateSystem::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckCoordinateSystem::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckCoordinateSystem::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) );
}
//=======================================================================================


//=======================================================================================
uint16_t CmdDisconnect::length()
{
    return 0;
}
//=======================================================================================
void CmdDisconnect::decode( vbyte_buffer_view* view )
{

}
//=======================================================================================
vbyte_buffer CmdDisconnect::encode()
{
    return vbyte_buffer();
}
//=======================================================================================
std::string CmdDisconnect::cat()
{
    return vcat();
}
//=======================================================================================


//=======================================================================================
uint16_t AckDisconnect::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void AckDisconnect::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckDisconnect::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckDisconnect::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) );
}
//=======================================================================================


//=======================================================================================
uint16_t MsgAbnormalStatusInfo::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void MsgAbnormalStatusInfo::decode(vbyte_buffer_view* view)
{
    status_code = view->u8();
}
//=======================================================================================
vbyte_buffer MsgAbnormalStatusInfo::encode()
{
    vbyte_buffer buf;

    buf.append( status_code );

    return buf;
}
//=======================================================================================
std::string MsgAbnormalStatusInfo::cat()
{
    return vcat( " | status_code: ", int( status_code ) );
}
//=======================================================================================


//=======================================================================================
uint16_t CmdConfigureIP::length()
{
    return sizeof ( uint8_t ) + 3 * sizeof ( uint32_t );
}
//=======================================================================================
void CmdConfigureIP::decode(vbyte_buffer_view* view)
{
    ip_mode = view->u8();
    ip_addr = view->u32_BE();
    net_mask = view->u32_BE();
    gw_addr = view->u32_BE();
}
//=======================================================================================
vbyte_buffer CmdConfigureIP::encode()
{
    vbyte_buffer buf;

    buf.append( ip_mode );
    buf.append_BE( ip_addr );
    buf.append_BE( net_mask );
    buf.append_BE( gw_addr );

    return buf;
}
//=======================================================================================
std::string CmdConfigureIP::cat()
{
    return vcat( " | ip_mode: ", int( ip_mode ),
                 " | ip_addr: ", int( ip_addr ),
                 " | net_mask: ", int( net_mask ),
                 " | gw: ", int( gw_addr ) );
}
//=======================================================================================


//=======================================================================================
uint16_t AckConfigureIP::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void AckConfigureIP::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckConfigureIP::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckConfigureIP::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) );
}
//=======================================================================================


//=======================================================================================
uint16_t CmdDeviceIPInfo::length()
{
    return 0;
}
//=======================================================================================
void CmdDeviceIPInfo::decode(vbyte_buffer_view* view)
{

}
//=======================================================================================
vbyte_buffer CmdDeviceIPInfo::encode()
{
    return vbyte_buffer();
}
//=======================================================================================
std::string CmdDeviceIPInfo::cat()
{
    return vcat();
}
//=======================================================================================


//=======================================================================================
uint16_t AckDeviceIPInfo::length()
{
    return 2 * sizeof ( uint8_t ) + 3 * sizeof ( uint32_t );
}
//=======================================================================================
void AckDeviceIPInfo::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
    ip_mode = view->u8();
    ip_addr = view->u32_LE();
    net_mask = view->u32_LE();
    gw = view->u32_LE();
}
//=======================================================================================
vbyte_buffer AckDeviceIPInfo::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );
    buf.append( ip_mode );
    buf.append_LE( ip_addr );
    buf.append_LE( net_mask );
    buf.append_LE( gw );

    return buf;
}
//=======================================================================================
std::string AckDeviceIPInfo::cat()
{
    return vcat( " | ret_code: ", int( ret_code ),
                 " | ip_mode: ", int( ip_mode ),
                 " | ip_addr: ", int( ip_addr ),
                 " | net_mask: ", int( net_mask ),
                 " | gw: ", int( gw ) );
}
//=======================================================================================


//=======================================================================================
uint16_t CmdRebootDevice::length()
{
    return sizeof ( uint16_t );
}
//=======================================================================================
void CmdRebootDevice::decode( vbyte_buffer_view* view )
{
    timeout = view->u16_LE();
}
//=======================================================================================
vbyte_buffer CmdRebootDevice::encode()
{
    vbyte_buffer buf;

    buf.append_LE( timeout );

    return buf;
}
//=======================================================================================
std::string CmdRebootDevice::cat()
{
    return vcat( " | timeout: ", int( timeout ) );
}
//=======================================================================================


//=======================================================================================
uint16_t AckRebootDevice::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void AckRebootDevice::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckRebootDevice::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckRebootDevice::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) );
}
//=======================================================================================
