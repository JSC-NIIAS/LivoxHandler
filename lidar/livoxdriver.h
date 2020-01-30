#ifndef LIVOXDRIVER_H
#define LIVOXDRIVER_H

#include "defs.h"
#include "customscatter.h"
#include "config.h"
#include "converter.h"

#include "vlog.h"
#include "vcat.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkDatagram>

//=======================================================================================
class LivoxDriver : public QObject
{
    Q_OBJECT

public:

    explicit LivoxDriver( Config& config,
                          const BroabcastInfo& info,
                          QObject* parent = nullptr );

    //-----------------------------------------------------------------------------------

private:

    Config *_conf = nullptr;
    BroabcastInfo _info;
    CustomScatter *_scatter = nullptr;
    QList<LivoxSpherPoint> _pnts;

    bool _was_lidar_init = false;

    QUdpSocket *_sock_data = nullptr;
    QUdpSocket *_sock_cmd = nullptr;
    QTimer *_heart_timer = nullptr;
    QTimer *_data_timer = nullptr;

    uint16_t _seq_num;

    //-----------------------------------------------------------------------------------

public slots:

    void _init_listen_ports();
    void _on_command();
    void _on_data();
    void _init_lidar();

    // General Command Set

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

#endif // LIVOXDRIVER_H
