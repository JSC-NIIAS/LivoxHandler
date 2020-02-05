#ifndef LIVOXDRIVER_H
#define LIVOXDRIVER_H

#include "defs.h"
#include "config.h"
#include "converter.h"
#include "livoxcontainer.h"
#include "livoxmessages.h"

#include "vlog.h"
#include "vcat.h"
#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"
#include "vtime_point.h"

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

signals:

    void transmit_point_cloud( const Pack&, const int32_t );
    void transmit_info( const LidarErrorCode&, const uint64_t );
    void transmit_imu( const LivoxImuPoint& );
    void transmit_packet_pnts( const QList<Package<LivoxSpherPoint>>& );

    //-----------------------------------------------------------------------------------

private:

    Config *_conf = nullptr;
    BroabcastInfo _info;
    LivoxContainer *_container = nullptr;

    bool _was_lidar_init = false;

    QUdpSocket *_sock_data = nullptr;
    QUdpSocket *_sock_cmd = nullptr;
    QTimer *_heart_timer = nullptr;

    uint16_t _seq_num;

    uint8_t _dev_type;

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
    void _set_return_mode( const PointCloudReturnMode mode );

};
//=======================================================================================

#endif // LIVOXDRIVER_H
