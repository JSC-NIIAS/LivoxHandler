#include "vlog.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"

#include <QApplication>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QNetworkInterface>

#include <livox_sdk.h>

using namespace std;

//=======================================================================================

int main( int argc, char **argv )
{
    vbyte_buffer ip_addr;

    ip_addr.append( uchar(192) );
    ip_addr.append( uchar(168) );
    ip_addr.append( uchar(150) );
    ip_addr.append( uchar(78)  );

    vdeb.hex() << ip_addr.view().u32_BE();

    QApplication qapp( argc, argv );


    return qapp.exec();
}
//=======================================================================================
