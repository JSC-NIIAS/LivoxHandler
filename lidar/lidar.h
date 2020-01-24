#ifndef LIDAR_H
#define LIDAR_H

#include "defs.h"

#include "vlog.h"
#include "vcat.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>

//=======================================================================================

class Lidar : public QObject
{
    Q_OBJECT

public:

    explicit Lidar( QObject* parent = nullptr );

    //-----------------------------------------------------------------------------------

signals:

    void handshake();

    void heartbeat();

    //-----------------------------------------------------------------------------------

private:

    QUdpSocket *_sock_data = nullptr;
    QUdpSocket *_sock_cmd = nullptr;

    QTimer *_heart_timer = nullptr;

    QHostAddress *_host_ip = nullptr;
    QHostAddress *_livox_ip = nullptr;

    static uint16_t _seq_num;

    //-----------------------------------------------------------------------------------

private slots:

    void _on_broadcast();

    void _on_handshake();
    void _to_handshake();

    void _to_heartbeat();
};

//=======================================================================================

#endif
