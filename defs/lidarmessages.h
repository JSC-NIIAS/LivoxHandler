#ifndef LIDARMESSAGES_H
#define LIDARMESSAGES_H

#include "livox_def.h"
#include "command_impl.h"

#include "vlog.h"
#include "vcat.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"
#include "vtime_point.h"

#include <string>

// List of Livox messages for General Command Set

//=======================================================================================
/* CmdSetMode
 * cmd_set: 0x01
 * cmd_id : 0x00
 * lidar_mode:
 *      User can configure as follow:
 *      0x01: Normal Mode
 *      0x02: Power-Saving Mode
 *      0x03: Standby Mode
*/
struct CmdSetMode
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarSetMode;

    uint8_t lidar_mode = 0x03;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckSetMode
 * cmd_set: 0x01
 * cmd_id : 0x00
 * ret_code:
 *      0x00: Success
 *      0x01: Fail
 *      0x02: Switching
*/
struct AckSetMode
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarSetMode;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdWriteExtrinsicParams
 * cmd_set: 0x01
 * cmd_id : 0x01
 * roll  : Roll Angle, Unit: degree
 * pitch : Pitch Angle, Unit: degree
 * yaw   : Yaw Angle, Unit: degree
 * x : X Translation, Unit: mm
 * y : Y Translation, Unit: mm
 * z : Z Translation, Unit: mm
*/
struct CmdWriteExtrinsicParams
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarSetExtrinsicParameter;

    float roll = 0.0;
    float pitch = 0.0;
    float yaw = 0.0;
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckWriteExtrinsicParams
 * cmd_set : 0x01
 * cmd_id  : 0x01
 * ret_code:
 *      0x00: Success
 *      0x01: Fail
*/
struct AckWriteExtrinsicParams
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarSetExtrinsicParameter;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdReadExtrinsicParams
 * cmd_set : 0x01
 * cmd_id  : 0x02
*/
struct CmdReadExtrinsicParams
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarGetExtrinsicParameter;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckReadExtrinsicParams
 * cmd_set : 0x01
 * cmd_id : 0x02
 * ret_code :
 *      0x00: Success
 *      0x01: Fail
 * roll  : Roll Angle, Unit: degree
 * pitch : Pitch Angle, Unit: degree
 * yaw   : Yaw Angle, Unit: degree
 * x : X Translation, Unit: mm
 * y : Y Translation, Unit: mm
 * z : Z Translation, Unit: mm
*/
struct AckReadExtrinsicParams
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarGetExtrinsicParameter;

    uint8_t ret_code = 0x01;
    float roll = 0.0;
    float pitch = 0.0;
    float yaw = 0.0;
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdWeatherSuppression
 * cmd_set : 0x01
 * cmd_id  : 0x03
 * Turn On/Off Rain/Fog Suppression:
 *      0x00: Turn Off
 *      0x01: Turn On
*/
struct CmdWeatherSuppression
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarControlRainFogSuppression;

    uint8_t state = 0x00;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckWeatherSuppression
 * cmd_set : 0x01
 * cmd_id : 0x03
 * ret_code :
 *      0x00: Success
 *      0x01: Fail
*/
struct AckWeatherSuppression
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarControlRainFogSuppression;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdTurnFan
 * cmd_set : 0x01
 * cmd_id : 0x04
 * Turn On/Off Fan:
 *      0x00: Turn Off
 *      0x01: Turn On
*/
struct CmdTurnFan
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarControlFan;

    uint8_t state = 0x00;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* cmd_set : 0x01
 * cmd_id : 0x04
 * ret_code:
 *      0x00: Success
 *      0x01: Fail
 * This configuration is saved in Flash
*/
struct AckTurnFan
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarControlFan;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdTurnFanState
 * cmd_set : 0x01
 * cmd_id : 0x05
*/
struct CmdTurnFanState
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarGetFanState;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckTurnFanState
 * cmd_set : 0x01
 * cmd_id : 0x05
 * ret_code:
 *      0x00: Success
 *      0x01: Fail
 * Turn On/Off Fan State: state
 *      0x00: Turn Off
 *      0x01: Turn On
*/
struct AckTurnFanState
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarGetFanState;

    uint8_t ret_code = 0x01;
    uint8_t state = 0x00;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdSetReturnMode
 * cmd_set : 0x01
 * cmd_id : 0x06
 * Return Mode: mode
 *      0x00: Single Return First
 *      0x01: Single Return Strongest
 *      0x02: Dual Return
*/
struct CmdSetReturnMode
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarSetPointCloudReturnMode;

    uint8_t mode = 0x00;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckSetReturnMode
 * cmd_set : 0x01
 * cmd_id : 0x06
 * ret_code :
 *      0x00: Success
 *      0x01: Fail
 * This configuration is saved in Flash
*/
struct AckSetReturnMode
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarSetPointCloudReturnMode;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdGetReturnMode
 * cmd_set : 0x01
 * cmd_id  : 0x07
*/
struct CmdGetReturnMode
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarGetPointCloudReturnMode;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckGetReturnMode
 * cmd_set : 0x01
 * cmd_id : 0x07
 * ret_code :
 *      0x00: Success
 *      0x01: Fail
 * Return Mode: mode
 *      0x00: Single Return First
 *      0x01: Single Return Strongest
 *      0x02: Dual Return
*/
struct AckGetReturnMode
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarGetPointCloudReturnMode;

    uint8_t ret_code = 0x01;
    uint8_t mode = 0x00;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdSetImuDataFreequency
 * Supported devices:
 * Horizon/06.04.0000+
 * Tele-15/07.03.0000+
 *
 * cmd_set : 0x01
 * cmd_id : 0x08
 * IMU Data Push Frequency: frequency
 *      0x00: 0Hz(Close IMU push)
 *      0x01: 200Hz
*/
struct CmdSetImuDataFreequency
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarSetImuPushFrequency;

    uint8_t frequency = 0x00;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckSetImuDataFreequency
 * cmd_set : 0x01
 * cmd_id : 0x08
 * ret_code:
 *      0x00: Success
 *      0x01: Fail
 * This configuration is saved in Flash.
*/
struct AckSetImuDataFreequency
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarSetImuPushFrequency;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdGetImuDataFreequency
 * Supported devices:
 * Horizon/06.04.0000+
 * Tele-15/07.03.0000+
 *
 * cmd_set : 0x01
 * cmd_id : 0x09
*/
struct CmdGetImuDataFreequency
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarGetImuPushFrequency;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckGetImuDataFreequency
 * cmd set: 0x01
 * cmd_id : 0x09
 * Return Code:
 *      0x00: Success
 *      0x01: Fail
 * IMU Data Push Frequency: frequency
 *      0x00: 0Hz(Close IMU Data Push)
 *      0x01: 200Hz
*/
struct AckGetImuDataFreequency
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarGetImuPushFrequency;

    uint8_t ret_code = 0x01;
    uint8_t frequency = 0x00;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdUpdateUTCSync
 * Supported devices:
 * Mid-40/03.07.0000+
 * Horizon/06.04.0000+
 * Tele-15/07.03.0000+
 *
 * cmd_set : 0x01
 * cmd_id : 0x0A
 * UTC Year: year
 *      0 is 2000 year
 *      255 is 2255 year
 * month : UTC Month: Range is 1-12
 * day : UTC Day: Range is 1-31
 * hour : UTC Hour: Range is 0-23
 * microsecond : 1 Hour Microsecond Offset
*/
struct CmdUpdateUTCSync
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarSetSyncTime;

    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint32_t microsecond;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckUpdateUTCSync
 * cmd_set : 0x01
 * cmd_id : 0x0A
 * Return Code:
 *      0x00: Success
 *      0x01: Fail
*/
struct AckUpdateUTCSync
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarSetSyncTime;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================

#endif // LIDARMESSAGES_H
