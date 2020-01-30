#ifndef LIVOXBROADCASTER_H
#define LIVOXBROADCASTER_H

#include "config.h"
#include "defs.h"

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QNetworkDatagram>

//=======================================================================================

struct BroabcastInfo
{
    QHostAddress address;
    int          port;
    QString      broadcast_code;
};

//=======================================================================================
class LivoxBroadcaster : public QObject
{
    Q_OBJECT

public:

    explicit LivoxBroadcaster( Config& config,
                               const QHostAddress& address,
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
