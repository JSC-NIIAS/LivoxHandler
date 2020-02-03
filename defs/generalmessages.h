#ifndef GENERALMESSAGES_H
#define GENERALMESSAGES_H

#include "livox_def.h"
#include "command_impl.h"

#include "vlog.h"
#include "vcat.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"

#include <string>

// List of Livox messages for General Command Set

//=======================================================================================
/* MsgBroadcastMessage:
 * cmd_set = 0x00
 * cmd_id = 0x00
 * broadcast_code - Device Broadcast Code of Each Device is UNIQUE. Ex: 0TFDFCE00502151
 * dev_type: 0 - Livox Hub
 *           1 - Mid-40
 *           2 - Tele-15
 *           3 - Horizon
*/
struct MsgBroadcast
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralBroadcast;

    std::string broadcast_code {};
    uint8_t     dev_type;
    uint16_t    reserved;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat() const;
};
//=======================================================================================


//=======================================================================================
/* CmdHandshake:
 * cmd_set = 0x00
 * cmd_id = 0x01
 * user_ip - Host IP Address. Example: AA.BB.CC.DD
 *           [0] = AA, [1] = BB, [2] = CC, [3] = DD
 * data_port - Host Point Cloud Data UDP Destination Port
 * cmd_port - Host Control Command UDP Destination Port
 * imu_port - Host Control IMU UDP Destination Port,
 *            Supported Devices:
 *              Horizon/06.04.0000+
 *              Tele-15/07.03.0000+
*/
struct CmdHandshake
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralHandshake;

    uint32_t user_ip;
    uint16_t data_port;
    uint16_t cmd_port;
    uint16_t imu_port;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckHandshake:
 * cmd_set = 0x00
 * cmd_id = 0x01
 * ret_code - 0x00: Success,
 *            0x01: Fail
*/
struct AckHandshake
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralHandshake;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdQueryDeviceInfo:
 * cmd_set = 0x00
 * cmd_id  = 0x02
*/
struct CmdQueryDeviceInfo
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralDeviceInfo;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckQueryDeviceInfo:
 * cmd_set = 0x00
 * cmd_id  = 0x02
 * ret_code - 0x00: Success,
 *            0x01: Fail
 * version - Firmware Version: Example: AA.BB.CC.DD
*/
struct AckQueryDeviceInfo
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralDeviceInfo;

    uint8_t  ret_code;
    uint32_t version;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* The host needs to send request data at a frequency of 1 Hz to maintain normal
 * connection with the device.
 * CmdHeartbeat:
 * cmd_set = 0x00
 * cmd_id  = 0x03
*/
struct CmdHeartbeat
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralHeartbeat;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckHeartbeat:
 * cmd_set = 0x00
 * cmd_id  = 0x03
 * ret_code - Return Code:
 *              0x00: Success
 *              0x01: Fail
 * work_state - LiDAR State:
 *                  0x00: Initializing
 *                  0x01: Normal
 *                  0x02: Power-Saving
 *                  0x03: Standby
 *                  0x04: Error
 *              Hub State:
 *                  0x00: Initializing
 *                  0x01: Normal
 *                  0x04: Error
 * feature_msg - LiDAR Feature Message:
 *                  Bit0: Rain/Fog Suppression Switch
 *                  0x00: Turn Off
 *                  0x01: Turn On
 *                  Bit1 ~ Bit7: Reserved
 *                  Hub is Reserved
 * ack_msg - ACK Message:
 *              Related to ‘work_state’:
 *              work_state is 0x00: Initialization Progress
 *              work_state is other: Status Code
*/
struct AckHeartbeat
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralHeartbeat;

    uint8_t  ret_code = 0x01;
    uint8_t  work_state = 0x04;
    uint8_t  feature_msg = 0x00;
    uint32_t ack_msg; // is 0x00 - Initialization Progress, other - Status Code

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdSampling:
 * cmd_set = 0x00
 * cmd_id  = 0x04
 * sample_ctrl : 0x00: Stop Sampling
 *               0x01: Start Sampling
*/
struct CmdSampling
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralControlSample;

    uint8_t sample_ctrl = 0x00;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckSampling:
 * cmd_set = 0x00
 * cmd_id  = 0x04
 * ret_code - Return Code:
 *              0x00: Success
 *              0x01: Fail
*/
struct AckSampling
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralControlSample;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* NOTE: This cmd only takes effect in RAM,
 * the coordinate system will reset to Cartesian after reboot.
 * CmdCoordinateSystem:
 * cmd_set = 0x00
 * cmd_id  = 0x05
 * coordinate_type : 0x00: Cartesian Coordinate
 *                   0x01: Spherical Coordinate
 *
*/
struct CmdCoordinateSystem
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralCoordinateSystem;

    uint8_t coordinate_type = 0x00;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckCoordinateSystem:
 * cmd_set = 0x00
 * cmd_id  = 0x05
 * ret_code - Return Code:
 *              0x00: Success
 *              0x01: Fail
*/
struct AckCoordinateSystem
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralCoordinateSystem;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdDisconnect:
 * cmd_set = 0x00
 * cmd_id  = 0x06
*/
struct CmdDisconnect
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralDisconnect;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckDisconnect:
 * cmd_set = 0x00
 * cmd_id  = 0x06
 * ret_code - Return Code:
 *              0x00: Success
 *              0x01: Fail
*/
struct AckDisconnect
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralDisconnect;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* When error occurs, the message is immediately pushed at 10Hz to report the abnormal
 * information.
 * MsgAbnormalStatusInfo:
 * cmd_set = 0x00
 * cmd_id  = 0x07
 * status_code
*/
struct MsgAbnormalStatusInfo
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralPushAbnormalState;

    uint8_t status_code;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================

//=======================================================================================
/* CmdConfigureIP:
 * cmd_set = 0x00
 * cmd_id  = 0x08
 * ip_mode : 0x00: Dynamic IP
 *           0x01: Static IP
 * ip_addr : IP Address
 * net_mask : IPV4 netmask
 *            IP netmask is valid when ‘ip_mode’ is 0x01(Static IP)
 *              Example: AA.BB.CC.DD
 *              net_mask[0]: AA
 *              net_mask[1]: BB
 *              net_mask[2]: CC
 *              net_mask[3]: DD
 *          Supported devices:
 *              Horizon/06.04.0000+
 *              Tele-15/07.03.0000+
 * gw_addr : IPV4 gateway
 *              IP gateway is valid when ‘ip_mode’ is 0x01(Static IP)
 *              Example: AA.BB.CC.DD
 *              gw_addr[0]: AA
 *              gw_addr[1]: BB
 *              gw_addr[2]: CC
 *              gw_addr[3]: DD
 *          Supported devices:
 *              Horizon/06.04.0000+
 *              Tele-15/07.03.0000+
*/
struct CmdConfigureIP
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralConfigureStaticDynamicIp;

    uint8_t ip_mode = 0x00;
    uint32_t ip_addr;
    uint32_t net_mask;
    uint32_t gw_addr;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckConfigureIP:
 * cmd_set = 0x00
 * cmd_id  = 0x08
 * ret_code - Return Code:
 *              0x00: Success
 *              0x01: Fail
*/
struct AckConfigureIP
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralConfigureStaticDynamicIp;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdDeviceIPInfo:
 * cmd_set = 0x00
 * cmd_id  = 0x09
*/
struct CmdDeviceIPInfo
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralGetDeviceIpInformation;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckDeviceIPInfo:
 * cmd_set = 0x00
 * cmd_id  = 0x09
 * ret_code :
 * ip_mode : 0x00: Dynamic IP
 *           0x01: Static IP
 * ip_addr : IP Address
 * net_mask : IPV4 netmask
 *            IP netmask is valid when ‘ip_mode’ is 0x01(Static IP)
 *              Example: AA.BB.CC.DD
 *              net_mask[0]: AA
 *              net_mask[1]: BB
 *              net_mask[2]: CC
 *              net_mask[3]: DD
 *          Supported devices:
 *              Livox Hub/08.07.0000+
 *              Horizon/06.04.0000+
 *              Tele-15/07.03.0000+
 * gw : Device IPV4 gateway
 *          Example: AA.BB.CC.DD
 *          user_gw[0] = AA
 *          user_gw[1] = BB
 *          user_gw[2] = CC
 *          user_gw[3] = DD
 *      Supported devices:
 *          Livox Hub/08.07.0000+
 *          Horizon/06.04.0000+
 *          Tele-15/07.03.0000+
*/
struct AckDeviceIPInfo
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralGetDeviceIpInformation;

    uint8_t ret_code = 0x01;
    uint8_t ip_mode = 0x00;
    uint32_t ip_addr;
    uint32_t net_mask;
    uint32_t gw;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================


//=======================================================================================
/* CmdRebootDevice
 *
 * Supported devices/firmware version:
 * Livox Hub/08.07.0000+
 * Mid-40/03.07.0000+
 * Horizon/06.04.0000+
 * Tele-15/07.03.0000+
 *
 * cmd_set = 0x00
 * cmd_id  = 0x0A
 * timeout : Reboot device delay time ( ms )
*/
struct CmdRebootDevice
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralRebootDevice;

    uint16_t timeout;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================
/* AckRebootDevice
 *
 * cmd_set = 0x00
 * cmd_id  = 0x0A
 * ret_code - Return Code:
 *              0x00: Success
 *              0x01: Fail
*/
struct AckRebootDevice
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralRebootDevice;

    uint8_t ret_code = 0x01;

    //-----------------------------------------------------------------------------------

    static uint16_t length();

    void decode( vbyte_buffer_view* view );

    vbyte_buffer encode();

    std::string cat();
};
//=======================================================================================

#endif // GENERALMESSAGES_H
