#if WITH_GUI
#include <QApplication>
#else
#include <QCoreApplication>
#endif

#include <iostream>

#include <zcm/zcm-cpp.hpp>

#include "vapplication.h"
#include "vlog.h"
#include "vfilelog.h"
#include "vgit.h"

#include "config.h"
#include "lidar.h"

using namespace std;

//=======================================================================================

int main( int argc, char** argv )
{
    VApplication app( argc, argv );

    VFileLog_Shared flog ( app.app_name() + ".log", 1e6, 3 );

    flog.register_self();

    //-----------------------------------------------------------------------------------

    //  Print conf & exit

    if ( app.args().has_flag( "--printconf" ) )
    {
        std::cout << Config::get() << std::endl;
        return 0;
    }

    //-----------------------------------------------------------------------------------

    // Print version & exit

    if ( app.args().has_flag( "--version" ) || app.args().has_flag( "-V" ) )
    {
        std::cout << "\n\n" << VGit::as_message() << "\n\n";

        return 0;
    }

    //-----------------------------------------------------------------------------------

    if ( app.args().has_flag( "--vgit" ) )
        std::cout << "\n\n" << VGit::as_message() << "\n\n";

    //-----------------------------------------------------------------------------------

    //  Reading config

    if ( !app.args().has_flag("-c") )
        vwarning << "Cannot find param '-c' configname, using default (%).";

    auto confname = app.args().take_std_value_or( "-c", "%" );

    if ( confname == "%" )
        confname = app.full_app_name() + ".conf";

    auto config = Config::load( confname );

    //-----------------------------------------------------------------------------------

    //  Storing PID into filename

    {
        if ( !app.args().has_flag( "-p" ) )
            vwarning << "Cannot find param '-p' pidname, using default (%).";

        auto pidname = app.args().take_std_value_or( "-p", "%" );

        if ( pidname == "%" )
            pidname = app.app_name() + ".pid";

        app.pid().store( config.main_params.pid_path, pidname );
    }

    // ----------------------------------------------------------------------------------

#if WITH_GUI

    QApplication qapp( argc, argv );

#else

    QCoreApplication qapp( argc, argv );

#endif

    // ----------------------------------------------------------------------------------

    Lidar& read_lidar = Lidar::get_instance();

    VecStr br_str = { config.receive.broadcast };

    int ret = read_lidar.init_lidar( br_str );
    if ( !ret )
      vdeb << "Init lds lidar success!";

    else
      vwarning << "Init lds lidar fail!";

    vdeb << "Start discovering device.\n";

    read_lidar.deinit_lidar();

    vdeb << "Livox lidar demo end!";

    // ----------------------------------------------------------------------------------



    // ----------------------------------------------------------------------------------

    return qapp.exec();
}
//=======================================================================================
