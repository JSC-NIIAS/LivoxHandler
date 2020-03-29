#include "config.h"
#include "livoxbroadcaster.h"
#include "livoxdriver.h"
#include "livoxmessages.h"

#include "vlog.h"
#include "vgit.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"

#ifdef GUI
#include <QApplication>
#else
#include <QCoreApplication>
#endif

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QStringList>
#include <QSettings>
#include <QString>
#include <QMap>

using namespace std;

//=======================================================================================

using DriverList = QMap<QString, LivoxDriver*>;

//=======================================================================================
int main( int argc, char **argv )
{
    QCoreApplication::setOrganizationName( "JSC NIIAS" );

#ifdef GUI
    QApplication qapp( argc, argv );
#else
    QCoreApplication qapp( argc, argv );
#endif

    //-----------------------------------------------------------------------------------

    auto qargs = qapp.arguments();

    auto cfg_path = qargs.at( qargs.indexOf( "-c" ) + 1 );

    //-----------------------------------------------------------------------------------

    Config config( cfg_path );

    //-----------------------------------------------------------------------------------

    if ( qargs.contains( "--help" ) )
    {
        vdeb << "Help: \n"
             << "-p           - pid name \n"
             << "-c           - config path \n"
             << "--vgit       - print version \n"
             << "-V           - print version & exit \n"
             << "--version    - print version & exit \n"
             << "--print_conf - print config & exit \n"
             << "--save_conf  - save config alongside the binary & exit \n";
        return 0;
    }

    //-----------------------------------------------------------------------------------

    //  Print conf & exit

    if ( qargs.contains( "--print_conf" ) )
    {
        config.deb();
        return 0;
    }

    //-----------------------------------------------------------------------------------

    // Save config as file & exit

    if ( qargs.contains( "--save_conf" ) )
    {
        config.to_file( qapp.applicationFilePath() );
        return 0;
    }

    //-----------------------------------------------------------------------------------

    // Print version & exit

    if ( qargs.contains( "-V" ) || qargs.contains( "--version" ) )
    {
        vdeb << vgit::as_message() << "\n\n";
        return 0;
    }

    //-----------------------------------------------------------------------------------

    if ( qargs.contains( "--vgit" ) )
        vdeb << vgit::as_message() << "\n\n";

    //-----------------------------------------------------------------------------------

    // For listen all livox datagrams in network area
    LivoxBroadcaster lbroadcast( config, &qapp );

    // List of connected devices
    DriverList broabcast_list;

    // Start listen datagrams on local port 55000
    QObject::connect( &lbroadcast, &LivoxBroadcaster::receive,
                      [&]( const BroabcastInfo& info )
    {
        if ( !config.contains( info.broadcast_code ) )
        {
            vdeb << "The Config doesn't contain" << info.broadcast_code;
            return - 1;
        }

        else if ( config.contains( info.broadcast_code ) &&
                  !broabcast_list.contains( info.broadcast_code ) )
        {
            vdeb << "Register new Livox Driver with broadcast: " << info.broadcast_code;
            broabcast_list.insert( info.broadcast_code,
                                   new LivoxDriver( config, info, &qapp ) );
        }

        else if ( config.contains( info.broadcast_code ) &&
                  broabcast_list.contains( info.broadcast_code ) )
            vwarning << "The Driver List already contains Device with broadcast!"
                     << info.broadcast_code;

        return 0;
    } );

    return qapp.exec();
}
//=======================================================================================
