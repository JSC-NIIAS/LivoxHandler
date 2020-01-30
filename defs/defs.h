#ifndef DEFS_H
#define DEFS_H

#include "vlog.h"
#include "vcat.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"

#include "livox_def.h"
#include "command_impl.h"
#include "sdk_protocol.h"

#include <QtGlobal>
#include <QList>

#include <string>

//=======================================================================================

static constexpr quint32 seed_16 = 0x4c49;
static constexpr quint32 seed_32 = 0x564f580a;
static constexpr auto heartbeat_ms = 800;
static constexpr auto data_ms = 100;

//=======================================================================================
static uint16_t calc_crc16( const char* buf, size_t len )
{
    static FastCRC16 fast_crc16 { seed_16 };

    auto ptr = static_cast<const uchar*>( static_cast<const void*>( buf ) );

    return fast_crc16.mcrf4xx_calc( ptr, uint16_t( len ) );
}
//=======================================================================================
static uint32_t calc_crc32( const char* buf, size_t len )
{
    static FastCRC32 fast_crc32 { seed_32 };

    auto ptr = static_cast<const uchar*>( static_cast<const void*>( buf ) );

    return fast_crc32.crc32_calc( ptr, uint16_t( len ) );
}
//=======================================================================================
static bool check_crc16( const char* buf, uint len )
{
    static FastCRC16 fast_crc16 { seed_16 };

    auto ptr = static_cast<const uchar*>( static_cast<const void*>( buf ) );

    return fast_crc16.mcrf4xx_calc( ptr, uint16_t( len ) ) == 0;
}
//=======================================================================================

//=======================================================================================

enum
{
    local_bcast_port = 55000,
    host_data_port   = 56001,
    host_cmd_port    = 55501,
    host_imu_port    = 56001,
    livox_port       = 65000
};

//=======================================================================================

typedef enum
{
    stop  = 0,
    start = 1
} LidarSample;

typedef enum
{
    off = 0,
    on  = 1
} Turn;

//=======================================================================================
struct Cmd
{
    uint8_t cmd_set;
    uint8_t cmd_id;

    Cmd( const uint8_t set = {}, const uint8_t id = {} )
        : cmd_set ( set ) ,
          cmd_id  ( id )
    {

    }

    uint16_t size() const
    {
        return sizeof ( Cmd );
    }

    void decode( vbyte_buffer_view* view )
    {
        cmd_set = view->u8();
        cmd_id  = view->u8();
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf;

        buf.append( cmd_set );
        buf.append( cmd_id );

        return buf;
    }

    std::string cat()
    {
        return vcat( " | cmd_set: ", int( cmd_set ), " | cmd_id: ", int( cmd_id ) );
    }
};

//=======================================================================================

struct BroabcastMessage
{
    Cmd         cmd;
    std::string broadcast_code;
    uint8_t     dev_type;
    uint16_t    reserved;

    BroabcastMessage( const Cmd& cmd = {},
                      const std::string code = {},
                      const uint8_t type = {} )
        : cmd            ( cmd  )
        , broadcast_code ( code )
        , dev_type       ( type )
    {

    }

    void decode( vbyte_buffer_view* view )
    {
        //cmd.decode( view );
        broadcast_code = view->string( kBroadcastCodeSize );
        broadcast_code.pop_back(); // kill zero.
        dev_type = view->u8();
        reserved = view->u16_LE();
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf = cmd.encode();

        buf.append( broadcast_code );
        buf.append( dev_type );
        buf.append_LE( reserved );

        return buf;
    }

    std::string cat()
    {
        return vcat( " | Cmd: ", cmd.cat(),
                     " | broadcast_code: ", broadcast_code,
                     " | dev_type: ", int( dev_type ),
                     " | reserved: ", int(reserved) );
    }
};

//=======================================================================================

struct CmdHandshake
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralHandshake;

    uint32_t user_ip;
    uint16_t data_port;
    uint16_t cmd_port;
    uint16_t imu_port;

    static uint16_t length()
    {   //Хрен знает как посчитано, взял из sdk.
        //return 10;
        return sizeof( user_ip   ) +
                sizeof( data_port ) * 3;
    }

    //    void decode( vbyte_buffer_view* view )
    //    {
    //        //cmd.decode( view );
    //        user_ip   = view->u32_LE();
    //        data_port = view->u16_LE();
    //        cmd_port  = view->u16_LE();
    //        imu_port  = view->u16_LE();
    //    }

    void encode( vbyte_buffer* buf ) const
    {
        buf->append_BE( user_ip   );
        buf->append_LE( data_port );
        buf->append_LE( cmd_port  );
        buf->append_LE( imu_port  );
    }
};

//=======================================================================================

struct AckHandshake
{
    Cmd     cmd;
    uint8_t ret_code;

    AckHandshake( const Cmd& cmd = {},
                  const uint8_t code = {} )
        : cmd      ( cmd  )
        , ret_code ( code )
    {

    }

    void decode( vbyte_buffer_view* view )
    {
        cmd.decode( view );
        ret_code = view->u8();
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf = cmd.encode();

        buf.append( ret_code );

        return buf;
    }

    std::string cat()
    {
        return vcat( " | Cmd: ", cmd.cat(),
                     " | ret_code: ", int( ret_code ) );
    }
};

//=======================================================================================

struct CmdQueryDeviceInfo
{
    Cmd cmd;

    CmdQueryDeviceInfo( const Cmd& cmd = {} )
        : cmd ( cmd )
    {

    }

    void decode( vbyte_buffer_view* view )
    {
        cmd.decode( view );
    }

    vbyte_buffer encode()
    {
        return cmd.encode();
    }
};

struct AckQueryDeviceInfo
{
    Cmd      cmd;
    uint8_t  ret_code;
    uint32_t version;

    AckQueryDeviceInfo( const Cmd& cmd = {},
                        const uint8_t code = {},
                        const uint32_t version = {} )
        : cmd      ( cmd     )
        , ret_code ( code    )
        , version  ( version )
    {

    }

    void decode( vbyte_buffer_view* view )
    {
        cmd.decode( view );
        ret_code = view->u8();
        version = view->u32_LE();
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf = cmd.encode();

        buf.append( ret_code );
        buf.append_LE( version );

        return buf;
    }
};

//=======================================================================================

struct CmdHeartbeat
{
    Cmd cmd;

    CmdHeartbeat( const Cmd& cmd = {} )
        : cmd ( cmd )
    {

    }

    uint16_t size() const
    {   //Хрен знает как посчитано, взял из sdk.
        return 15;
    }

    void decode( vbyte_buffer_view* view )
    {
        cmd.decode( view );
    }

    vbyte_buffer encode()
    {
        return cmd.encode();
    }
};

//=======================================================================================

struct AckHeartbeat
{
    Cmd      cmd;
    uint8_t  ret_code;
    uint8_t  work_state;
    uint8_t  feature_msg;
    uint32_t ack_msg;

    AckHeartbeat( const Cmd& cmd = {},
                  const uint8_t code = {},
                  const uint8_t state = {},
                  const uint8_t fmsg = {},
                  const uint32_t amsg = {} )
        : cmd         ( cmd   )
        , ret_code    ( code  )
        , work_state  ( state )
        , feature_msg ( fmsg  )
        , ack_msg     ( amsg  )
    {

    }

    void decode( vbyte_buffer_view* view )
    {
        cmd.decode( view );
        ret_code = view->u8();
        work_state = view->u8();
        feature_msg = view->u8();
        ack_msg = view->u8();
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf = cmd.encode();

        buf.append( ret_code );
        buf.append( work_state );
        buf.append( feature_msg );
        buf.append_LE( ack_msg );

        return buf;
    }
};

//=======================================================================================

struct CmdSampling
{
    Cmd     cmd;
    uint8_t sampling_ctrl;

    CmdSampling();

    CmdSampling( const Cmd& cmd = {}, const uint8_t ctrl = {} )
        : cmd           ( cmd  )
        , sampling_ctrl ( ctrl )
    {

    }

    void decode( vbyte_buffer_view* view )
    {
        cmd.decode( view );
        sampling_ctrl = view->u8();
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf = cmd.encode();

        buf.append( sampling_ctrl );

        return buf;
    }
};

struct AckSampling
{
    Cmd      cmd;
    uint8_t  ret_code;

    AckSampling( const Cmd& cmd = {}, const uint8_t code = {} )
        : cmd      ( cmd  )
        , ret_code ( code )
    {

    }

    void decode( vbyte_buffer_view* view )
    {
        cmd.decode( view );
        ret_code = view->u8();
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf = cmd.encode();

        buf.append( ret_code );

        return buf;
    }
};

//=======================================================================================

struct CmdCoordinateSystem
{
    Cmd     cmd;
    uint8_t coordinate_type;

    CmdCoordinateSystem( const Cmd& cmd = {}, const uint8_t type = {} )
        : cmd             ( cmd  )
        , coordinate_type ( type )
    {

    }

    void decode( vbyte_buffer_view* view )
    {
        cmd.decode( view );
        coordinate_type = view->u8();
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf = cmd.encode();

        buf.append( coordinate_type );

        return buf;
    }
};

struct AckCoordinateSystem
{
    Cmd      cmd;
    uint8_t  ret_code;

    AckCoordinateSystem( const Cmd& cmd = {}, const uint8_t type = {} )
        : cmd      ( cmd  )
        , ret_code ( type )
    {

    }

    void decode( vbyte_buffer_view* view )
    {
        cmd.decode( view );
        ret_code = view->u8();
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf = cmd.encode();

        buf.append( ret_code );

        return buf;
    }
};

//=======================================================================================

struct HeartBeat2
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralHeartbeat;

    static uint16_t length() { return 0; }

    void encode( vbyte_buffer*) const {}
};

struct Sampling
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralControlSample;

    uint8_t sample_ctrl;

    static uint16_t length() { return 1; }

    void encode( vbyte_buffer* buf) const
    {
        buf->append( sample_ctrl );
    }
};

struct CoordinateSystem
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralCoordinateSystem;
    uint8_t coordinate_type = PointDataType::kCartesian;

    static uint16_t length() { return 1; }

    void encode( vbyte_buffer* buf) const
    {
        buf->append( coordinate_type );
    }
};

struct DevInfo
{
    static constexpr uint8_t cmd_set = livox::kCommandSetGeneral;
    static constexpr uint8_t cmd_id  = livox::kCommandIDGeneralDeviceInfo;

    static uint16_t length() { return 0; }

    void encode( vbyte_buffer*) const {}
};

struct Mode
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarSetMode;
    uint8_t lidar_mode = LidarMode::kLidarModeStandby;

    static uint16_t length() { return 1; }

    void encode( vbyte_buffer* ) const {}
};

struct LidarExtrinsicParameters
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarSetExtrinsicParameter;

    LidarSetExtrinsicParameterRequest params;

    static uint16_t length() { return sizeof( LidarSetExtrinsicParameterRequest ); }

    void encode( vbyte_buffer* ) const {}
};

struct LidarWeatherSuppress
{
    static constexpr uint8_t cmd_set = livox::kCommandSetLidar;
    static constexpr uint8_t cmd_id  = livox::kCommandIDLidarControlRainFogSuppression;

    uint8_t state = Turn::off;

    static uint16_t length() { return sizeof( uint8_t ); }

    void encode( vbyte_buffer* ) const {}
};

struct Head
{
    //  Три поля выставляем на encode()
    uint8_t  version = livox::kSdkVer0;
    uint8_t  cmd_type;
    uint16_t seq_num;

    //  Три поля заполянются при decode()
    uint8_t  cmd_set = -1;
    uint8_t  cmd_id  = -1;
    int      length  = -1;

    std::string str() const
    {
        return vcat("Head(v:", int(version), ",t:", int(cmd_type),
                    ",set:",int(cmd_set), ",id:", int(cmd_id), ",len:", length, ")" );
    }

    template<typename T>
    vbyte_buffer encode( const T& data )
    {
        vbyte_buffer buf;

        buf.append   ( uint8_t(0xAA) );
        buf.append   ( version       );
        buf.append_LE( uint16_t(T::length() + _heap_len()) );
        buf.append   ( cmd_type      );
        buf.append_LE( seq_num       );

        uint16_t crc16 = calc_crc16( buf.str().c_str(), buf.size() );
        buf.append_LE( crc16 );

        buf.append( T::cmd_set );
        buf.append( T::cmd_id  );

        data.encode( &buf );

        uint32_t crc32 = calc_crc32( buf.str().c_str(), buf.str().size() );
        buf.append_LE( crc32 );

        return buf;
    }

    bool decode( vbyte_buffer_view * view )
    {
        if ( view->remained() < size_t( _heap_len() - 4 ) ) return false;
        auto for_crc = view->show_string( _heap_for_crc16() );
        auto calc_crc = calc_crc16( for_crc.c_str(), _heap_for_crc16() );

        auto sof = view->u8();
        if ( sof != 0xAA ) return false;

        version  = view->u8();
        length   = view->u16_LE();
        cmd_type = view->u8();
        seq_num  = view->u16_LE();

        auto crc = view->u16_LE();
        if ( crc != calc_crc ) return false;

        cmd_set = view->u8();
        cmd_id  = view->u8();

        return true;
    }

private:
    static uint16_t _heap_len()       { return 15; }
    static size_t   _heap_for_crc16() { return 7;  }
};

template <typename T>
struct Frame
{
    uint8_t  version;
    uint8_t  cmd_type;
    uint16_t seq_num;

    uint8_t  cmd_set = 0xFF;
    uint8_t  cmd_id  = 0xFF;

    T        data;
    //uint32_t crc_32;

    static uint16_t length()
    {
        return T::length() + 15;
    }

    bool decode( vbyte_buffer_view* view )
    {
        auto sof = view->u8();
        if ( sof != 0xAA ) return false;

        version = view->u8();
        auto length = view->u16_LE();

        cmd_type = view->u8();
        seq_num  = view->u16_LE();

        auto crc_16 = view->u16_LE();
        (void) crc_16; // сделать проверку CRC16 head

        cmd_set = view->u8();
        cmd_id  = view->u8();

        data.decode( view );

        auto crc_32 = view->u32_LE();
        (void)crc_32; // Проверить CRC32.

        assert( view->finished() );

        return true;
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf;

        buf.append   ( uint8_t(0xAA) );
        buf.append   ( version       );
        buf.append_LE( length()      );
        buf.append   ( cmd_type      );
        buf.append_LE( seq_num       );

        uint16_t crc16 = calc_crc16( buf.str().c_str(), buf.size() );
        buf.append_LE( crc16 );

        buf.append( T::cmd_set );
        buf.append( T::cmd_id  );
        data.encode( &buf );

        uint32_t crc32 = calc_crc32( buf.str().c_str(), buf.str().size() );
        buf.append_LE( crc32 );

        return buf;
    }

    std::string cat()
    {
        return vcat( //" | sof: ", int( sof ),
                     " | version: ", int( version ),
                     //" | length: ", length,
                     " | cmd_type: ", int( cmd_type ),
                     " | seq_num: ", seq_num,
                     //" | crc_16: ",  crc_16,
                     " | T: ", data.cat()
                     //" | crc_32: ", std::hex, crc_32
                     );

    }
};

//=======================================================================================

// Point property based on spatial position
struct PointSpatialPosition
{
    uint8_t normal    = 0x00; // Normal
    uint8_t high_conf = 0x01; // High confidence level of the noise
    uint8_t mode_conf = 0x10; // Moderate confidence level of the noise
    uint8_t low_conf  = 0x11; // Low confidence level of the noise
};

// Point property based on intensity
struct PointIntensity
{
    uint8_t normal    = 0x00; // Normal
    uint8_t high_conf = 0x01; // High confidence level of the noise
    uint8_t mode_conf = 0x10; // Moderate confidence level of the noise
    uint8_t reserved;
};

union StatusData
{
    LidarErrorCode err;
    uint32_t _status;

    LidarErrorCode fill( const uint32_t status )
    {
        LidarErrorCode res;

        _status = status;

        res.temp_status = err.temp_status;
        res.volt_status = err.volt_status;
        res.motor_status = err.motor_status;
        res.dirty_warn = err.dirty_warn;
        res.firmware_err = err.firmware_err;
        res.pps_status = err.pps_status;
        res.device_status = err.device_status;
        res.fan_status = err.fan_status;
        res.self_heating = err.self_heating;
        res.ptp_status = err.ptp_status;
        res.time_sync_status = err.time_sync_status;
        res.rsvd = err.rsvd;
        res.system_status = err.system_status;

        return res;
    }
};

#include <type_traits>

template <typename P>
struct Package
{
    static auto constexpr delta_ns = 10;

    uint8_t version;
    uint8_t slot_id;
    uint8_t lidar_id;
    uint8_t reserved;
    LidarErrorCode status_code;
    uint8_t timestamp_type;
    uint8_t data_type;
    uint64_t timestamp;

    QList<LivoxSpherPoint> pnts;

    bool decode( vbyte_buffer_view* view )
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
    }

    std::string cat()
    {
        return vcat( " | version: ", int( version ),
                     " | slot_id: ", int( slot_id ),
                     " | lidar_id: ", int( lidar_id ),
                     " | status_code: ", int( status_code.system_status ),
                     " | timestamp_type: ",  int( timestamp_type ),
                     " | data_type: ", int( data_type ),
                     " | timestamp: ", std::chrono::nanoseconds( timestamp )
                     );

    }
};

//=======================================================================================

#include <QHostAddress>
struct BroabcastInfo
{
    QHostAddress address;
    int          port;
    QString      broadcast_code;
    uint16_t     seq_num;
};
//=======================================================================================

#endif
