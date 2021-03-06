#ifndef LIVOXMESSAGES_H
#define LIVOXMESSAGES_H

#include "defs.h"
#include "generalmessages.h"
#include "lidarmessages.h"
#include "hubmessages.h"

#include "vlog.h"
#include "vcat.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"

#include "livox_def.h"
#include "command_impl.h"
#include "sdk_protocol.h"

#include <string>

//=======================================================================================
union StatusData
{
    LidarErrorCode err;
    uint32_t _status;

    //-----------------------------------------------------------------------------------

    LidarErrorCode fill( const uint32_t status );
};
//=======================================================================================

//=======================================================================================
template<typename T>
struct Frame
{
    //  Три поля выставляем на encode()
    uint8_t  version = livox::kSdkVer0;
    uint8_t  cmd_type;
    uint16_t seq_num;

    //  Три поля заполянются при decode()
    uint8_t  cmd_set = T::cmd_set;
    uint8_t  cmd_id  = T::cmd_id;
    int      length  = 0;

    T data;

    //-----------------------------------------------------------------------------------

    static uint16_t size()
    {
        return T::length() + 15;
    }

    vbyte_buffer encode()
    {
        vbyte_buffer buf;

        buf.append   ( uint8_t( 0xAA ) );
        buf.append   ( version       );
        buf.append_LE( uint16_t( T::length() + _heap_len() ) );
        buf.append   ( cmd_type      );
        buf.append_LE( seq_num       );

        uint16_t crc16 = calc_crc16( buf.str().c_str(), buf.size() );
        buf.append_LE( crc16 );

        buf.append( T::cmd_set );
        buf.append( T::cmd_id  );

        buf.append( data.encode() );

        uint32_t crc32 = calc_crc32( buf.str().c_str(), buf.str().size() );
        buf.append_LE( crc32 );

        return buf;
    }

    bool decode( vbyte_buffer_view* view )
    {
        if ( view->remained() < size_t( _heap_len() - 4 ) )
            return false;

        auto for_crc = view->show_string( _heap_for_crc16() );
        auto calc_crc = calc_crc16( for_crc.c_str(), _heap_for_crc16() );

        auto sof = view->u8();
        if ( sof != 0xAA )
            return false;

        version  = view->u8();
        length   = view->u16_LE();
        cmd_type = view->u8();
        seq_num  = view->u16_LE();

        auto crc = view->u16_LE();
        if ( crc != calc_crc )
            return false;

        cmd_set = view->u8();
        cmd_id  = view->u8();

        data.decode( view );

        return true;
    }

    std::string cat() const
    {
        return vcat( "Frame:: version: ", int( version ),
                     ", cmd_type:", int( cmd_type ),
                    ", cmd_set:", int( cmd_set ),
                     ", cmd_id:", int( cmd_id ),
                     ", length:", length,
                     ", Data:", data.cat(),
                     ")" ).str();
    }

    //-----------------------------------------------------------------------------------

private:

    static uint16_t _heap_len()
    {
        return 15;
    }

    static size_t   _heap_for_crc16()
    {
        return 7;
    }

};
//=======================================================================================

//=======================================================================================
template <typename T>
struct Package
{
    uint8_t version;
    uint8_t slot_id;
    uint8_t lidar_id;
    uint8_t reserved;
    LidarErrorCode status_code;
    uint8_t timestamp_type;
    uint8_t data_type;
    uint64_t timestamp;

    QList<T> pnts;

    //-----------------------------------------------------------------------------------

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

        auto ost = view->remained() % sizeof (T);

        while ( !view->finished() && ( ost == 0 ) )
        {
            T pnt;

            pnt.depth = view->u32_LE();
            pnt.theta = view->u16_LE();
            pnt.phi   = view->u16_LE();
            pnt.reflectivity = view->u8();

            pnts.push_back( pnt );
        }

        return true;
    }

    std::string cat()
    {
        return vcat( " | version: ",         int( version ),
                     " | slot_id: ",         int( slot_id ),
                     " | lidar_id: ",        int( lidar_id ),
                     " | status_code: ",     int( status_code.system_status ),
                     " | timestamp_type: ",  int( timestamp_type ),
                     " | data_type: ",       int( data_type ),
                     " | timestamp: ",       std::chrono::nanoseconds( timestamp )
                     ).str();
    }

};
//=======================================================================================

#endif // LIVOXMESSAGES_H
