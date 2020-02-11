#ifndef CONFIG_H
#define CONFIG_H

#include "vlog.h"

#include <QObject>
#include <QString>
#include <QSettings>
#include <QHostAddress>

//=======================================================================================

class Config

{
public:

    Config( const QString& fname = {} );

    static void to_file( const QString& fname = {} );

    bool contains( const QString& broadcast );

    void deb();

    //-----------------------------------------------------------------------------------

    QHostAddress ip() const;
    void ip( const QHostAddress& );
    void ip_default();

    //-----------------------------------------------------------------------------------

    struct receive
    {
        QString broadcast;

    } receive;

    //-----------------------------------------------------------------------------------

    struct zcm_send
    {
        QString target;
        QString prefix;
        QString data_channel;
        QString imu_channel;
        QString info_channel;

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

    struct main_params
    {
        QString ip;
        QString pid_path;
        bool    need_trace;
        int     data_freequency;
        int     imu_freequency;
        int     info_freequency;

    } main_params;

    //-----------------------------------------------------------------------------------

    struct offset
    {
        float roll;
        float pitch;
        float yaw;
        int32_t x;
        int32_t y;
        int32_t z;

    } offset;

    //-----------------------------------------------------------------------------------

    struct lidar_params
    {
        bool weather_suppress;

    } lidar_params;

    //-----------------------------------------------------------------------------------

private:

    QSettings *_settings = nullptr;

    QString _default_path;

    QStringList _broadcast_list;

    QHostAddress _ip;

    //-----------------------------------------------------------------------------------

    void _make_default_path();
};

//=======================================================================================

#endif // CONFIG_H
