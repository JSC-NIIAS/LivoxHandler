#include "lidarmessages.h"

//=======================================================================================
uint16_t CmdSetMode::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void CmdSetMode::decode( vbyte_buffer_view* view )
{
    lidar_mode = view->u8();
}
//=======================================================================================
vbyte_buffer CmdSetMode::encode()
{
    vbyte_buffer buf;

    buf.append( lidar_mode );

    return buf;
}
//=======================================================================================
std::string CmdSetMode::cat()
{
    return vcat( " | lidar_mode: ", int( lidar_mode ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t AckSetMode::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void AckSetMode::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckSetMode::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckSetMode::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) ).str();
}
//=======================================================================================


//=======================================================================================

uint16_t CmdWriteExtrinsicParams::length()
{
    return 3 * ( sizeof ( float ) + sizeof ( int32_t ) );
}
//=======================================================================================
void CmdWriteExtrinsicParams::decode( vbyte_buffer_view* view )
{
    roll = view->float_LE();
    pitch = view->float_LE();
    yaw = view->float_LE();
    x = view->i32_LE();
    y = view->i32_LE();
    z = view->i32_LE();
}
//=======================================================================================
vbyte_buffer CmdWriteExtrinsicParams::encode()
{
    vbyte_buffer buf;

    buf.append_LE( roll );
    buf.append_LE( pitch );
    buf.append_LE( yaw );
    buf.append_LE( x );
    buf.append_LE( y );
    buf.append_LE( z );

    return buf;
}
//=======================================================================================
std::string CmdWriteExtrinsicParams::cat()
{
    return vcat( " | roll: ", float( roll ),
                 " | pitch: ", float( pitch ),
                 " | yaw: ", float( yaw ),
                 " | x: ", int( x ),
                 " | y: ", int( y ),
                 " | z: ", int( z ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t AckWriteExtrinsicParams::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void AckWriteExtrinsicParams::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckWriteExtrinsicParams::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckWriteExtrinsicParams::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t CmdReadExtrinsicParams::length()
{
    return 0;
}
//=======================================================================================
void CmdReadExtrinsicParams::decode( vbyte_buffer_view* view )
{

}
//=======================================================================================
vbyte_buffer CmdReadExtrinsicParams::encode()
{
    return vbyte_buffer();
}
//=======================================================================================
std::string CmdReadExtrinsicParams::cat()
{
    return vcat().str();
}
//=======================================================================================


//=======================================================================================
uint16_t AckReadExtrinsicParams::length()
{
    return sizeof ( uint8_t ) + 3 * ( sizeof ( float ) + sizeof ( int32_t ) );
}
//=======================================================================================
void AckReadExtrinsicParams::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
    roll = view->float_LE();
    pitch = view->float_LE();
    yaw = view->float_LE();
    x = view->i32_LE();
    y = view->i32_LE();
    z = view->i32_LE();
}
//=======================================================================================
vbyte_buffer AckReadExtrinsicParams::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );
    buf.append_LE( roll );
    buf.append_LE( pitch );
    buf.append_LE( yaw );
    buf.append_LE( x );
    buf.append_LE( y );
    buf.append_LE( z );

    return buf;
}
//=======================================================================================
std::string AckReadExtrinsicParams::cat()
{
    return vcat( " | ret_code: ", int( ret_code ),
                 " | roll: ", float( roll ),
                 " | pitch: ", float( pitch ),
                 " | yaw: ", float( yaw ),
                 " | x: ", int( x ),
                 " | y: ", int( y ),
                 " | z: ", int( z ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t CmdWeatherSuppression::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void CmdWeatherSuppression::decode( vbyte_buffer_view* view )
{
    state = view->u8();
}
//=======================================================================================
vbyte_buffer CmdWeatherSuppression::encode()
{
    vbyte_buffer buf;

    buf.append( state );

    return buf;
}
//=======================================================================================
std::string CmdWeatherSuppression::cat()
{
    return vcat( " | state: ", int( state ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t AckWeatherSuppression::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void AckWeatherSuppression::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckWeatherSuppression::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckWeatherSuppression::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t CmdTurnFan::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void CmdTurnFan::decode( vbyte_buffer_view* view )
{
    state = view->u8();
}
//=======================================================================================
vbyte_buffer CmdTurnFan::encode()
{
    vbyte_buffer buf;

    buf.append( state );

    return buf;
}
//=======================================================================================
std::string CmdTurnFan::cat()
{
    return vcat( " | state: ", int( state ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t AckTurnFan::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void AckTurnFan::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckTurnFan::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckTurnFan::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t CmdTurnFanState::length()
{
    return 0;
}
//=======================================================================================
void CmdTurnFanState::decode( vbyte_buffer_view* view )
{

}
//=======================================================================================
vbyte_buffer CmdTurnFanState::encode()
{
    return vbyte_buffer();
}
//=======================================================================================
std::string CmdTurnFanState::cat()
{
    return vcat().str();
}
//=======================================================================================


//=======================================================================================
uint16_t CmdSetReturnMode::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void CmdSetReturnMode::decode( vbyte_buffer_view* view )
{
    mode = view->u8();
}
//=======================================================================================
vbyte_buffer CmdSetReturnMode::encode()
{
    vbyte_buffer buf;

    buf.append( mode );

    return buf;
}
//=======================================================================================
std::string CmdSetReturnMode::cat()
{
    return vcat( " | mode: ", int( mode ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t AckSetReturnMode::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void AckSetReturnMode::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckSetReturnMode::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckSetReturnMode::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t CmdGetReturnMode::length()
{
    return 0;
}
//=======================================================================================
void CmdGetReturnMode::decode( vbyte_buffer_view* view )
{

}
//=======================================================================================
vbyte_buffer CmdGetReturnMode::encode()
{
    return vbyte_buffer();
}
//=======================================================================================
std::string CmdGetReturnMode::cat()
{
    return vcat().str();
}
//=======================================================================================


//=======================================================================================
uint16_t AckGetReturnMode::length()
{
    return 2 * sizeof ( uint8_t );
}
//=======================================================================================
void AckGetReturnMode::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
    mode = view->u8();
}
//=======================================================================================
vbyte_buffer AckGetReturnMode::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );
    buf.append( mode );

    return buf;
}
//=======================================================================================
std::string AckGetReturnMode::cat()
{
    return vcat( " | ret_code: ", int( ret_code ),
                 " | mode: ", int( mode ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t CmdSetImuDataFreequency::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void CmdSetImuDataFreequency::decode( vbyte_buffer_view* view )
{
    frequency = view->u8();
}
//=======================================================================================
vbyte_buffer CmdSetImuDataFreequency::encode()
{
    vbyte_buffer buf;

    buf.append( frequency );

    return buf;
}
//=======================================================================================
std::string CmdSetImuDataFreequency::cat()
{
    return vcat( " | frequency: ", int( frequency ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t AckSetImuDataFreequency::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void AckSetImuDataFreequency::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckSetImuDataFreequency::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckSetImuDataFreequency::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t CmdGetImuDataFreequency::length()
{
    return 0;
}
//=======================================================================================
void CmdGetImuDataFreequency::decode( vbyte_buffer_view* view )
{

}
//=======================================================================================
vbyte_buffer CmdGetImuDataFreequency::encode()
{
    return vbyte_buffer();
}
//=======================================================================================
std::string CmdGetImuDataFreequency::cat()
{
    return vcat().str();
}
//=======================================================================================


//=======================================================================================
uint16_t AckGetImuDataFreequency::length()
{
    return 2 * sizeof ( uint8_t );
}
//=======================================================================================
void AckGetImuDataFreequency::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
    frequency = view->u8();
}
//=======================================================================================
vbyte_buffer AckGetImuDataFreequency::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );
    buf.append( frequency );

    return buf;
}
//=======================================================================================
std::string AckGetImuDataFreequency::cat()
{
    return vcat( " | ret_code: ", int( ret_code ),
                 " | frequency: ", int( frequency ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t CmdUpdateUTCSync::length()
{
    return 4 * sizeof ( uint8_t ) + sizeof ( uint32_t );
}
//=======================================================================================
void CmdUpdateUTCSync::decode( vbyte_buffer_view* view )
{
    year = view->u8();
    month = view->u8();
    day = view->u8();
    hour = view->u8();
    microsecond = view->u32_LE();
}
//=======================================================================================
vbyte_buffer CmdUpdateUTCSync::encode()
{
    vbyte_buffer buf;

    buf.append( year );
    buf.append( month );
    buf.append( day );
    buf.append( hour );
    buf.append_LE( microsecond );

    return buf;
}
//=======================================================================================
std::string CmdUpdateUTCSync::cat()
{
    return vcat( " | year: ", int( year ),
                 " | month: ", int( month ),
                 " | day: ", int( day ),
                 " | hour: ", int( hour ),
                 " | microsecond: ", std::chrono::microseconds( microsecond ) ).str();
}
//=======================================================================================


//=======================================================================================
uint16_t AckUpdateUTCSync::length()
{
    return sizeof ( uint8_t );
}
//=======================================================================================
void AckUpdateUTCSync::decode( vbyte_buffer_view* view )
{
    ret_code = view->u8();
}
//=======================================================================================
vbyte_buffer AckUpdateUTCSync::encode()
{
    vbyte_buffer buf;

    buf.append( ret_code );

    return buf;
}
//=======================================================================================
std::string AckUpdateUTCSync::cat()
{
    return vcat( " | ret_code: ", int( ret_code ) ).str();
}
//=======================================================================================
