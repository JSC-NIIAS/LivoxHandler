#ifndef CONFIG_H
#define CONFIG_H

#include "vlog.h"

#include <QObject>
#include <QString>
#include <QSettings>
#include <QHostAddress>

//=======================================================================================

class Config : public QObject

{
    Q_OBJECT

public:

    explicit Config( const QString& fname = {}, QObject* parent = nullptr );

    static void to_file( const QString& fname );

    //-----------------------------------------------------------------------------------

    QHostAddress ip() const;
    void ip( const QHostAddress& );

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
        QString channel;

        QString data_ch() const
        {
            return prefix + channel;
        }

    } zcm_send;

    //-----------------------------------------------------------------------------------

    struct main_params
    {
        QString pid_path;
        bool    need_trace;
        int     freequency;

    } main_params;

    //-----------------------------------------------------------------------------------

    struct offset
    {
        float x;
        float y;
        float z;
        float roll;
        float pitch;
        float yaw;

    } offset;

    //-----------------------------------------------------------------------------------

    struct lidar_params
    {
        bool weather_suppress;
        int  point_type;

    } lidar_params;

    //-----------------------------------------------------------------------------------

    bool contains( const QString& broadcast );

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
