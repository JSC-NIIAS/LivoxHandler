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

private:

    QUdpSocket *_sock_listener = nullptr;
    QUdpSocket *_sock_data = nullptr;
    QUdpSocket *_sock_cmd = nullptr;

    QTimer *_heart_timer = nullptr;

    QHostAddress _host_ip;
    QHostAddress _livox_ip;

    static uint16_t _seq_num;

    //-----------------------------------------------------------------------------------

private slots:

    void _on_broadcast();
    void _init_listen_ports();

    void _send_handshake();

    void _on_command();
    void _on_data();
    void _send_heartbeat();
};

//=======================================================================================

#endif
