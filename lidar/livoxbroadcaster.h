#ifndef LIVOXBROADCASTER_H
#define LIVOXBROADCASTER_H

#include "config.h"
#include "defs.h"
#include "livoxmessages.h"

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QNetworkDatagram>

//=======================================================================================
class LivoxBroadcaster : public QObject
{
    Q_OBJECT

public:

    explicit LivoxBroadcaster( Config& config,
                               QObject *parent = nullptr );

    //-----------------------------------------------------------------------------------

    void _get();

    //-----------------------------------------------------------------------------------

private:

    Config *_conf = nullptr;

    QUdpSocket *_sock_listener = nullptr;

    QHostAddress _local_ip;

    //-----------------------------------------------------------------------------------

signals:

    void receive( const BroabcastInfo& );

};
//=======================================================================================

#endif // LIVOXBROADCASTER_H
