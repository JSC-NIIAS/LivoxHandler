#ifndef CONFIG_H
#define CONFIG_H

//=======================================================================================

#include "vgio_keyfile_schema.h"

static constexpr auto keyfile_heap = "# --- Lidar Livox with  ---\n\n";

//=======================================================================================

class Config
{
public:

    static Config load( const std::string& fname );

    static std::string get ();

    //-----------------------------------------------------------------------------------

    struct receive
    {
        std::string broadcast;
        std::string ip;
        int32_t     port;

    } receive;

    //-----------------------------------------------------------------------------------

    struct zcm_send
    {
        std::string target;
        std::string zcm_ch_out;
        std::string zcm_ch_data;

        std::string data_ch() const
        {
            return zcm_ch_out + zcm_ch_data;
        }

    } zcm_send;

    //-----------------------------------------------------------------------------------

    struct main_params
    {
        std::string pid_path;
        bool        need_trace;
        bool        with_gui;

    } main_params;

    //-----------------------------------------------------------------------------------

    struct kinematic_model
    {
        float shift_x;
        float shift_y;
        float shift_z;

    } kinematic_params;

    //-----------------------------------------------------------------------------------

private:

    struct Schema
    {
        Schema( Config* conf )
        {
            auto& sh = schema;

            //---------------------------------------------------------------------------

            sh.set_current_group( "receive" );

            sh.append( "broadcast",
                       &conf->receive.broadcast, "0TFDFCE00502151" );
            sh.append( "ip",
                       &conf->receive.ip, "192.168.150.230" );
            sh.append( "port",
                       &conf->receive.port, 50005 );

            //---------------------------------------------------------------------------

            sh.set_current_group( "zcm_send" );

            sh.append( "target",
                       &conf->zcm_send.target, "ipc" );
            sh.append( "zcm_ch_out",
                       &conf->zcm_send.zcm_ch_out, "F" );
            sh.append( "zcm_ch_data",
                       &conf->zcm_send.zcm_ch_data, "LivoxScanData" );

            //---------------------------------------------------------------------------

            sh.set_current_group( "main_params" );

            sh.append( "pid_path",
                       &conf->main_params.pid_path, "/tmp/niias" );
            sh.append( "need_trace",
                       &conf->main_params.need_trace, true );
            sh.append( "with_gui",
                       &conf->main_params.with_gui, false );

            //---------------------------------------------------------------------------

            sh.set_current_group( "kinematic_params" );

            sh.append( "shift_x",
                       &conf->kinematic_params.shift_x, 0.0 );
            sh.append( "shift_y",
                       &conf->kinematic_params.shift_y, 0.0 );
            sh.append( "shift_z",
                       &conf->kinematic_params.shift_z, 0.0 );

            //---------------------------------------------------------------------------
        }

        vgio::KeyFile_Schema schema;

    }; // Schema
};

//=======================================================================================

#endif // CONFIG_H
