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

#include <string>

//=======================================================================================

static constexpr quint32 seed_16 = 0x4c49;
static constexpr quint32 seed_32 = 0x564f580a;
static constexpr auto heartbeat_ms = 800;

//=======================================================================================
static uint16_t calc_crc16( const char* buf, uint len )
{
    static FastCRC16 fast_crc16 { seed_16 };

    auto ptr = static_cast<const uchar*>( static_cast<const void*>( buf ) );

    return fast_crc16.mcrf4xx_calc( ptr, uint16_t( len ) );
}
//=======================================================================================
static uint32_t calc_crc32( const char* buf, uint len )
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

struct Cmd
{
    uint8_t cmd_set;
    uint8_t cmd_id;

    Cmd( const uint8_t set = {}, const uint8_t id = {} )
        : cmd_set ( set ) ,
          cmd_id  ( id )
    {

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
        cmd.decode( view );
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
    Cmd cmd;
    uint32_t user_ip;
    uint16_t data_port;
    uint16_t cmd_port;
    uint16_t imu_port;

    CmdHandshake( const Cmd& cmd = {},
                  const uint32_t ip = {},
                  const uint16_t data_p = {},
                  const uint16_t cmd_p = {},
                  const uint16_t imu_p = {} )
        : cmd       ( cmd    )
        , user_ip   ( ip     )
        , data_port ( data_p )
        , cmd_port  ( cmd_p  )
        , imu_port  ( imu_p  )
    {

    }

    void decode( vbyte_buffer_view* view )
    {
        cmd.decode( view );
        user_ip = view->u32_LE();
        data_port = view->u16_LE();
        cmd_port = view->u16_LE();
        imu_port = view->u16_LE();
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf = cmd.encode();

        buf.append_LE( user_ip );
        buf.append_LE( data_port );
        buf.append_LE( cmd_port );
        buf.append_LE( imu_port );

        return buf;
    }
};

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

    void decode( vbyte_buffer_view* view )
    {
        cmd.decode( view );
    }

    vbyte_buffer encode()
    {
        return cmd.encode();
    }
};

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

template <typename T>
struct Frame
{
    uint8_t  sof;
    uint8_t  version;
    uint16_t length;
    uint8_t  cmd_type;
    uint16_t seq_num;
    uint16_t crc_16;
    T        cmd;
    uint32_t crc_32;

    Frame( const uint8_t sof = {},
           const uint8_t version = {},
           const uint16_t length = {},
           const uint8_t cmd_type = {},
           const uint16_t seq_num = {},
           const uint16_t crc_16 = {},
           const T& cmd = {},
           const uint32_t crc_32 = {} )
        : sof      ( sof      )
        , version  ( version  )
        , length   ( length   )
        , cmd_type ( cmd_type )
        , seq_num  ( seq_num  )
        , crc_16   ( crc_16   )
        , cmd      ( cmd      )
        , crc_32   ( crc_32   )
    {

    }

    void decode( vbyte_buffer_view* view )
    {
        sof = view->u8();
        version = view->u8();
        length = view->u16_LE();
        cmd_type = view->u8();
        seq_num = view->u16_LE();
        crc_16 = view->u16_LE();
        (void) crc_16;
        cmd.decode( view );
//        view.omit(2);
        crc_32 = view->u32_LE();
        assert( view->finished() );
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf;

        buf.append( sof );
        buf.append( version );
        buf.append_LE( length );
        buf.append_LE( calc_crc16( buf.str().c_str(), uint( buf.str().size() ) ) );
        buf.append( cmd.encode() );
        buf.append_LE( calc_crc32( buf.str().c_str(), buf.str().size() ) );

        return buf;
    }

    std::string cat()
    {
        return vcat( " | sof: ", int( sof ),
                     " | version: ", int( version ),
                     " | length: ", int( length ),
                     " | cmd_type: ", int( cmd_type ),
                     " | seq_num: ", int( seq_num ),
                     " | crc_16: ", int( crc_16 ),
                     " | T: ", cmd.cat(),
                     " | crc_32: ", int( crc_32 ) );
    }
};

//=======================================================================================

enum
{
    host_data_port = 55000,
    host_cmd_port = 56000,
    host_imu_port = 55500,
    livox_port = 65000
};

//=======================================================================================

#endif
