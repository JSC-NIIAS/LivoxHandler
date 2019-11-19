#include <stdio.h>
#include <string.h>
#include <thread>
#include <memory>

#include "lidar.h"

using namespace std;

//=======================================================================================

/** User add broadcast code here */
static const char* local_broadcast_code_list[] = {
  "0TFDFCE00502151",
};

/** For callback use only */
Lidar* g_lidars = nullptr;

//=======================================================================================
Lidar& Lidar::get_instance()
{
    static Lidar lds_lidar;

    return lds_lidar;
}
//=======================================================================================
int Lidar::init_lidar( VecStr& broadcast_code_strs )
{
    if ( is_init_ )
    {
        vfatal << "LiDAR data source is already inited!";

        return - 1;
    }

    if ( !Init() )
    {
        Uninit();

        vfatal << "Livox-SDK init fail!";

        return -1;
    }

    LivoxSdkVersion _sdkversion;
    GetLivoxSdkVersion( &_sdkversion );
    vdeb << "Livox SDK version "
         << _sdkversion.major
         << _sdkversion.minor
         << _sdkversion.patch;

    SetBroadcastCallback( Lidar::on_device_broadcast );
    SetDeviceStateUpdateCallback( Lidar::on_device_change );

    /** Add commandline input broadcast code */

    for ( auto input_str : broadcast_code_strs )
      Lidar::add_broadcast_to_list( input_str.c_str() );

    /** Add local broadcast code */

    Lidar::add_local_broadcast();

    if ( whitelist_count_ )
    {
      Lidar::disable_auto_connect_mode();
      vdeb << "Disable auto connect mode!";

      vdeb << "List all broadcast code in whiltelist:";

      for ( uint32_t i = 0; i < whitelist_count_; i++ )
        vdeb << broadcast_code_whitelist_[i];
    }

    else
    {
      Lidar::enable_auto_connect_mode();

      vdeb << "No broadcast code was added to whitelist,"
           << "swith to automatic connection mode!";
    }

    /** Start livox sdk to receive lidar data */

    if ( !Start() )
    {
      Uninit();

      vdeb << "Livox-SDK init fail!";

      return - 1;
    }

    /** Add here, only for callback use */

    if ( g_lidars == nullptr )
      g_lidars = this;

    is_init_= true;

    vdeb << "Livox-SDK init success!";

    return 0;
}
//=======================================================================================
int Lidar::deinit_lidar()
{
    if ( !is_init_ )
    {
        vfatal << "LiDAR data source is not exit";
        return - 1;
    }

    Uninit();

    vdeb << "Livox SDK Deinit completely!";

    return 0;
}
//=======================================================================================


//=======================================================================================
Lidar::Lidar()
{
    auto_connect_mode_ = true;
    whitelist_count_   = 0;
    is_init_    = false;

    lidar_count_       = 0;
    memset( broadcast_code_whitelist_, 0, sizeof( broadcast_code_whitelist_ ) );

    memset( lidars_, 0, sizeof( lidars_ ) );

    for ( uint32_t i = 0; i < kMaxLidarCount; i++ )
    {
        lidars_[i].handle = kMaxLidarCount;
        lidars_[i].connect_state = kConnectStateOff;
    }
}
//=======================================================================================
Lidar::~Lidar()
{

}
//=======================================================================================
void Lidar::get_lidar_data_cb( uint8_t handle,
                               LivoxEthPacket* data,
                               uint32_t data_num,
                               void* client_data )
{
    using namespace std;

    Lidar* lidar_this = static_cast<Lidar*>( client_data );
    LivoxEthPacket* eth_packet = data;

    if ( !data || !data_num || ( handle >= kMaxLidarCount ) )
        return;

    if ( eth_packet )
    {
        lidar_this->data_recveive_count_[ handle ] += data_num;

        if ( lidar_this->data_recveive_count_[ handle ] % 10000 == 0)
        {
            vdeb << "Receive packet count "
                 << handle
                 << lidar_this->data_recveive_count_[ handle ];

            /** Parsing the timestamp and the point cloud data. */

            uint64_t cur_timestamp = *( ( uint64_t* )( data->timestamp ) );
            LivoxRawPoint *p_point_data = ( LivoxRawPoint* ) data->data;
        }
    }
}
//=======================================================================================


//=======================================================================================
void Lidar::on_device_broadcast( const BroadcastDeviceInfo* info )
{
    if ( info == nullptr )
        return;

    if ( info->dev_type == kDeviceTypeHub )
    {
        vwarning << "In lidar mode, couldn't connect a hub : "
                 << info->broadcast_code;
        return;
    }

    if ( g_lidars->is_auto_connect_mode() )
        vdeb << "In automatic connection mode, will connect %s\n"
             << info->broadcast_code;

    else
    {
        if ( !g_lidars->find_in_list( info->broadcast_code ) )
        {
            vwarning << "Not in the whitelist, please add "
                     << info->broadcast_code
                     << " to if want to connect!";

            return;
        }
    }

    bool result = false;
    uint8_t handle = 0;

    result = AddLidarToConnect( info->broadcast_code, &handle );

    if ( ( result == kStatusSuccess ) && ( handle < kMaxLidarCount ) )
    {
        SetDataCallback( handle, Lidar::get_lidar_data_cb, (void *)g_lidars );

        g_lidars->lidars_[ handle ].handle = handle;
        g_lidars->lidars_[ handle ].connect_state = kConnectStateOff;
    }

    else
        vwarning << "Add lidar to connect is failed :" << result << handle;
}
//=======================================================================================
void Lidar::on_device_change( const DeviceInfo* info, DeviceEvent type )
{
    if ( info == nullptr )
        return;

    uint8_t handle = info->handle;

    if ( handle >= kMaxLidarCount )
        return;

    LidarDevice* p_lidar = &( g_lidars->lidars_[ handle ] );

    if ( type == kEventConnect )
    {
        QueryDeviceInformation( handle, device_information_cb, g_lidars );

        if ( p_lidar->connect_state == kConnectStateOff )
        {
            p_lidar->connect_state = kConnectStateOn;
            p_lidar->info = *info;
        }
    }

    else if ( type == kEventDisconnect )
    {
        p_lidar->connect_state = kConnectStateOff;

        vwarning << "Lidar" << info->broadcast_code << "disconnect!\n";
    }

    else if ( type == kEventStateChange )
        p_lidar->info = *info;

    if ( p_lidar->connect_state == kConnectStateOn )
    {
        vdeb << "Lidar"
             << p_lidar->info.broadcast_code
             << "status_code"
             << p_lidar->info.status.status_code
             << "working state"
             << p_lidar->info.state
             << "feature"
             << p_lidar->info.feature;

        if ( p_lidar->info.state == kLidarStateNormal )
        {
            LidarStartSampling( handle, Lidar::start_sample_cb, g_lidars );
            p_lidar->connect_state = kConnectStateSampling;
        }
    }
}
//=======================================================================================


//=======================================================================================
void Lidar::start_sample_cb( uint8_t status,
                             uint8_t handle,
                             uint8_t response,
                             void* clent_data )
{
    Lidar* lds_lidar = static_cast<Lidar*>( clent_data );

    if ( handle >= kMaxLidarCount )
        return;

    LidarDevice* p_lidar = &( lds_lidar->lidars_[ handle ] );

    if ( status == kStatusSuccess )
    {
        if ( response != 0 )
        {
            p_lidar->connect_state = kConnectStateOn;
            vdeb << "Lidar start sample fail : state"
                 << status
                 << "handle"
                 << handle
                 << "res"
                 << response;
        }

        else
            vwarning << "Lidar start sample success";
    }

    else if ( status == kStatusTimeout )
    {
        p_lidar->connect_state = kConnectStateOn;

        vdeb << "Lidar start sample timeout : state"
             << status
             << "handle"
             << handle
             << "res"
             << response;
    }
}
//=======================================================================================
void Lidar::stop_sample_cb( uint8_t status,
                            uint8_t handle,
                            uint8_t response,
                            void* clent_data )
{

}

void Lidar::device_information_cb( uint8_t status,
                                   uint8_t handle,
                                   DeviceInformationResponse* ack,
                                   void* clent_data )
{
    if ( status != kStatusSuccess )
        vdeb << "Device Query Informations Failed : " << status;

    if ( ack )
    {
        vdeb << "firm ver:"
             << ack->firmware_version[0]
             << ack->firmware_version[1]
             << ack->firmware_version[2]
             << ack->firmware_version[3];
    }
}
//=======================================================================================


//=======================================================================================
int Lidar::add_broadcast_to_list( const char* bd_code )
{
    if ( !bd_code || ( strlen( bd_code ) > kBroadcastCodeSize ) ||
         ( whitelist_count_ >= kMaxLidarCount ) )
      return - 1;

    if ( Lidar::find_in_list( bd_code ) )
    {
      vdeb << bd_code << "is alrealy exist!";

      return - 1;
    }

    strcpy( broadcast_code_whitelist_[ whitelist_count_ ], bd_code );

    ++whitelist_count_;

    return 0;
}
//=======================================================================================
void Lidar::add_local_broadcast()
{
    for ( size_t i=0; i < sizeof( local_broadcast_code_list ) / sizeof( intptr_t ); ++i )
    {
      std::string invalid_bd = "000000000";

      vdeb << "Local broadcast code : " << local_broadcast_code_list[i];

      if ( ( kBroadcastCodeSize == strlen( local_broadcast_code_list[i] ) ) &&
          ( nullptr == strstr( local_broadcast_code_list[i], invalid_bd.c_str() ) ) )
        Lidar::add_broadcast_to_list( local_broadcast_code_list[i] );

      else
        vdeb << "Invalid local broadcast code : " << local_broadcast_code_list[i];
    }
}
//=======================================================================================
bool Lidar::find_in_list( const char* bd_code )
{
    if ( !bd_code )
      return false;

    for ( uint32_t i = 0; i < whitelist_count_; i++ )
      if ( strncmp( bd_code, broadcast_code_whitelist_[i], kBroadcastCodeSize ) == 0 )
        return true;

    return false;
}
//=======================================================================================
void Lidar::enable_auto_connect_mode()
{
    auto_connect_mode_ = true;
}
//=======================================================================================
void Lidar::disable_auto_connect_mode()
{
    auto_connect_mode_ = false;
}
//=======================================================================================
bool Lidar::is_auto_connect_mode()
{
    return auto_connect_mode_;
}
//=======================================================================================
