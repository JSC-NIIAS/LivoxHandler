#include "defs.h"
#include "lidar.h"
#include "config.h"

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

    Config config( cfg_path );

    //-----------------------------------------------------------------------------------

    QHostAddress local_ip;

    for ( const QHostAddress& address: QNetworkInterface::allAddresses() )
        if ( address.protocol() == QAbstractSocket::IPv4Protocol &&
             address != QHostAddress::LocalHost )
             local_ip = address;

    //-----------------------------------------------------------------------------------

    Lidar lidar( config, local_ip );

    return qapp.exec();
}
//=======================================================================================
