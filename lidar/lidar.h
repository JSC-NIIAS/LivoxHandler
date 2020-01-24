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
public:

    explicit Lidar( QObject* parent = nullptr );

    //-----------------------------------------------------------------------------------

signals:

    //-----------------------------------------------------------------------------------

private:

    QUdpSocket *_sock_data = nullptr;
    QUdpSocket *_sock_cmd = nullptr;

    QTimer *_heart_timer = nullptr;

    static uint16_t _seq_num;

    //-----------------------------------------------------------------------------------

private slots:

    void _on_broadcast();

};

//=======================================================================================

#endif
