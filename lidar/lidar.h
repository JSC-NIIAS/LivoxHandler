#ifndef LIDAR_H
#define LIDAR_H

#include <memory>
#include <vector>

#include "livox_def.h"
#include "livox_sdk.h"

#include "vlog.h"

#include "defs.h"

//=======================================================================================

typedef enum
{
    kConnectStateOff      = 0,
    kConnectStateOn       = 1,
    kConnectStateSampling = 2,
} LidarConnectState;

typedef struct
{
        uint8_t           handle;
        LidarConnectState connect_state;
        DeviceInfo        info;
} LidarDevice;

//=======================================================================================

class Lidar
{
    public:

        static Lidar& get_instance();

        int init_lidar( VecStr& broadcast );

        int deinit_lidar( void );

        //-------------------------------------------------------------------------------

    private:

        bool auto_connect_mode_;
        uint32_t whitelist_count_;
        volatile bool is_init_;
        char broadcast_code_whitelist_ [ kMaxLidarCount ][ kBroadcastCodeSize ];

        uint32_t lidar_count_;
        LidarDevice lidars_ [ kMaxLidarCount ];

        uint32_t data_recveive_count_ [ kMaxLidarCount ];

        //-------------------------------------------------------------------------------

        Lidar();
        Lidar( const Lidar& ) = delete;
        ~Lidar();

        //-------------------------------------------------------------------------------

        Lidar& operator=( const Lidar& ) = delete;

        static void get_lidar_data_cb( uint8_t handle,
                                       LivoxEthPacket* data,
                                       uint32_t data_num,
                                       void* client_data );

        //-------------------------------------------------------------------------------

        static void on_device_broadcast( const BroadcastDeviceInfo* info );

        static void on_device_change( const DeviceInfo* info, DeviceEvent type);

        //-------------------------------------------------------------------------------

        static void start_sample_cb( uint8_t status,
                                     uint8_t handle,
                                     uint8_t response,
                                     void* clent_data );

        static void stop_sample_cb( uint8_t status,
                                    uint8_t handle,
                                    uint8_t response,
                                    void* clent_data );

        static void device_information_cb( uint8_t status,
                                           uint8_t handle,
                                           DeviceInformationResponse* ack,
                                           void* clent_data );

        //-------------------------------------------------------------------------------

        int add_broadcast_to_list( const char* broadcast_code );

        void add_local_broadcast( void );

        bool find_in_list( const char* broadcast_code );

        //-------------------------------------------------------------------------------

        void enable_auto_connect_mode( void );

        void disable_auto_connect_mode( void );

        bool is_auto_connect_mode( void );

};

//=======================================================================================

#endif
