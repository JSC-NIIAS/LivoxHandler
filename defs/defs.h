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

#include <QHostAddress>
struct BroabcastInfo
{
    QHostAddress address;
    int          port;
    QString      broadcast_code;
    uint16_t     seq_num;
    uint8_t      dev_type;
};
//=======================================================================================

#endif
