#ifndef LIDAR_H
#define LIDAR_H

#include "defs.h"
#include "customscatter.h"
#include "config.h"

#include "vlog.h"
#include "vcat.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>
#include <QtMath>

//=======================================================================================

class Lidar : public QObject
{
    Q_OBJECT

public:

    explicit Lidar( Config& config,
                    const QHostAddress& address,
                    QObject* parent = nullptr );

    //-----------------------------------------------------------------------------------

private:

    Config *_conf = nullptr;

    QUdpSocket *_sock_listener = nullptr;
    QUdpSocket *_sock_data = nullptr;
    QUdpSocket *_sock_cmd = nullptr;

    QTimer *_heart_timer = nullptr;
    QTimer *_data_timer = nullptr;

    QHostAddress _host_ip;
    QHostAddress _livox_ip;

    static uint16_t _seq_num;

    QList<LivoxSpherPoint> _pnts;

    CustomScatter *_scatter = nullptr;

    bool _was_lidar_init = false;

    QList<LivoxRawPoint> _spherical_to_cartezian( const QList<LivoxSpherPoint>& pnts );

    //-----------------------------------------------------------------------------------

private slots:

    void _init_listen_ports();
    void _init_lidar();

    void _on_command();
    void _on_data();

    // General Command Set

    void _get_broadcast();
    void _set_handshake();
    void _set_heartbeat();
    void _sampling( const LidarSample sample );
    void _change_coord_system( const PointDataType type );

    // Lidar Command Set

    void _set_mode( const LidarMode mode );
    void _set_extr_params();
    void _set_weather_suppress( const Turn turn );
};

//=======================================================================================

#endif
