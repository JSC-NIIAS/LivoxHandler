#ifndef LIVOXCONTAINER_H
#define LIVOXCONTAINER_H

#ifdef WITH_GUI
#include "customscatter.h"
#endif

#include "defs.h"
#include "converter.h"
#include "config.h"
#include "livoxmessages.h"
#include "zcmpublish.h"

#include "livox_def.h"

#include <QObject>
#include <QTimer>

//=======================================================================================
class LivoxContainer : public QObject
{
    Q_OBJECT

public:

    explicit LivoxContainer( const Config& conf,
                             const QString& broadcast = {},
                             QObject* parent = nullptr );

    //-----------------------------------------------------------------------------------

signals:
    void transmit_packet_pnts( const QList<Pack>& );

public slots:

    void add_pack( const Pack& data, const int32_t ts );
    void set_imu( const LivoxImuPoint& pnt );
    void set_info( const LidarErrorCode& code, const uint64_t ts );

    //-----------------------------------------------------------------------------------

private:

#ifdef WITH_GUI
    CustomScatter *_scatter = nullptr;
#endif

    ZcmPublish *_pub = nullptr;

    QTimer *_data_timer = nullptr;
    QTimer *_imu_timer = nullptr;
    QTimer *_info_timer = nullptr;

    QList<Pack> _packs;
    int32_t capture_time;
    QList<LivoxImuPoint>   _imu_pnts;
    LidarStatus _sensor_info;

};
//=======================================================================================

#endif // LIVOXCONTAINER_H
