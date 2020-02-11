#ifndef ZCMPUBLISH_H
#define ZCMPUBLISH_H

#include "config.h"
#include "livoxmessages.h"

#include "vtime_point.h"
#include "vlog.h"

#include "livox_def.h"

#include "ZcmDeviceInfo.hpp"
#include "ZcmLivoxIMU.hpp"
#include "PointTag.hpp"
#include "ZcmLivoxPoint.hpp"
#include "ZcmLivoxScan.hpp"

#include <QObject>
#include <QString>
#include <QtMath>

#include "zcm/zcm-cpp.hpp"

//=======================================================================================

class ZcmPublish : public QObject
{
    Q_OBJECT

public:

    explicit ZcmPublish( const Config& conf );

    ~ZcmPublish();

    //-----------------------------------------------------------------------------------

public slots:

    void send_point_cloud( const QList<Pack>& data, const int32_t capture );
    void send_imu_data();
    void send_info( const LidarStatus& code, const QString& broadcast );

    //-----------------------------------------------------------------------------------

private:

    Config   _conf;
    zcm::ZCM _zcm;

};

//=======================================================================================

#endif // ZCMPUBLISH_H
