#include "defs.h"
#include "lidar.h"
#include "config.h"
#include "livoxbroadcaster.h"
#include "livoxdriver.h"

#include "vlog.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"

#include <QApplication>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QStringList>
#include <QSettings>

using namespace std;

using DriverList = QMap<QString, LivoxDriver*>;

//=======================================================================================
int main( int argc, char **argv )
{
    QApplication qapp( argc, argv );

    //-----------------------------------------------------------------------------------

    auto qargs = qapp.arguments();

    if ( qargs.contains( "-c" ) )
        qDebug() << qargs.indexOf( "-c" );

    auto cfg_path = qargs.at( qargs.indexOf( "-c" ) + 1 );

    //-----------------------------------------------------------------------------------

    Config config( cfg_path, &qapp );

    //-----------------------------------------------------------------------------------

    for ( const QHostAddress& address: QNetworkInterface::allAddresses() )
        if ( address.protocol() == QAbstractSocket::IPv4Protocol &&
             address != QHostAddress::LocalHost )
            config.ip( address );

    //-----------------------------------------------------------------------------------

    LivoxBroadcaster lbroadcast( config, &qapp );

    DriverList broabcast_list;

    QObject::connect( &lbroadcast, &LivoxBroadcaster::receive,
                      [&]( const BroabcastInfo& info )
    {
        if ( !config.contains( info.broadcast_code ) )
            return - 1;

        else if ( config.contains( info.broadcast_code ) &&
                  !broabcast_list.contains( info.broadcast_code ) )
            broabcast_list.insert( info.broadcast_code,
                                   new LivoxDriver( config, info, &qapp ) );

        else if ( config.contains( info.broadcast_code ) &&
                  broabcast_list.contains( info.broadcast_code ) )
            vwarning << "The Driver List already contains Device with broadcast!"
                     << info.broadcast_code;

        return 0;
    } );

    return qapp.exec();
}
//=======================================================================================
