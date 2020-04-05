#ifndef CONFIG_H
#define CONFIG_H

#include "vlog.h"
#include "vsettings.h"

#include <QString>
#include <QHostAddress>

//=======================================================================================

class Config
{
public:

    Config( const QString& fname = {} );

    //-----------------------------------------------------------------------------------

    void to_file( const QString& fname = {} );

    bool contains( const QString& broadcast );

    void deb();

    //-----------------------------------------------------------------------------------

    QHostAddress ip() const;
    void ip( const QHostAddress& );

    //-----------------------------------------------------------------------------------

    struct Receive
    {
        QString broadcast;

        std::string str = "receive";

    } receive;

    //-----------------------------------------------------------------------------------

    struct ZcmSend
    {
        QString target;
        QString prefix;
        QString data_channel;
        QString imu_channel;
        QString info_channel;

        std::string str = "zcm_send";

        QString data_ch() const
        {
            return prefix + data_channel;
        }

        QString imu_ch() const
        {
            return prefix + imu_channel;
        }

        QString info_ch() const
        {
            return prefix + info_channel;
        }

    } zcm_send;

    //-----------------------------------------------------------------------------------

    struct MainParams
    {
        QString ip;
        QString pid_path;
        int     data_freequency;
        int     imu_freequency;
        int     info_freequency;

        std::string str = "main_params";

    } main_params;

    //-----------------------------------------------------------------------------------

    struct LidarParams
    {
        bool weather_suppress;

        std::string str = "lidar_params";

    } lidar_params;

    //-----------------------------------------------------------------------------------

    struct Offset
    {
        float roll;
        float pitch;
        float yaw;
        int32_t x;
        int32_t y;
        int32_t z;

        std::string str = "offset";

    } offset;

    //-----------------------------------------------------------------------------------

    QMap<QString, Offset> & broadcasts();

    //-----------------------------------------------------------------------------------

private:

    vsettings *_settings = nullptr;

    QMap<QString, Offset> _broadcast_list;

    QHostAddress _ip;

    //-----------------------------------------------------------------------------------

    void _build();

};

//=======================================================================================

#endif // CONFIG_H
