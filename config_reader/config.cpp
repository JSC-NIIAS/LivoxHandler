#include "config.h"

#include <QCoreApplication>

//=======================================================================================
Config::Config( const QString& fname, QObject* parent )
    : QObject ( parent )
{
    if ( fname.isEmpty() )
        _make_default_path();
    else
        _default_path = fname;

    _settings = new QSettings( _default_path, QSettings::IniFormat, this );

    if ( _settings->status() != QSettings::NoError )
        throw verror << "No config specified for path: " << fname;

    {
        _settings->beginGroup( "receive" );
        receive.broadcast = _settings->value( "broadcast", "" ).toString();
        _settings->endGroup();

        _settings->beginGroup( "zcm_send" );
        zcm_send.target = _settings->value( "target", "" ).toString();
        zcm_send.prefix = _settings->value( "prefix", "" ).toString();
        zcm_send.channel = _settings->value( "channel", "" ).toString();
        _settings->endGroup();

        _settings->beginGroup( "main_params" );
        main_params.pid_path = _settings->value( "pid_path", "" ).toString();
        main_params.need_trace = _settings->value( "need_trace", "" ).toBool();
        main_params.freequency = _settings->value( "freequency", "" ).toInt();
        _settings->endGroup();

        _settings->beginGroup( "offset" );
        offset.roll = _settings->value( "roll", "" ).toFloat();
        offset.pitch = _settings->value( "pitch", "" ).toFloat();
        offset.yaw = _settings->value( "yaw", "" ).toFloat();
        offset.x = _settings->value( "x", "" ).toInt();
        offset.y = _settings->value( "y", "" ).toInt();
        offset.z = _settings->value( "z", "" ).toInt();
        _settings->endGroup();

        _settings->beginGroup( "lidar_params" );
        lidar_params.weather_suppress = _settings->value( "weather_suppress", "" ).toBool();
        _settings->endGroup();
    }

    _broadcast_list.append( receive.broadcast );
}
//=======================================================================================
void Config::to_file( const QString& fname )
{
    QSettings settings( fname, QSettings::IniFormat );

    settings.clear();

    {
        settings.beginGroup( "receive" );
        settings.setValue( "broadcast", QString( "0TFDFCE00502151" ) );
        settings.endGroup();
    }

    {
        settings.beginGroup( "zcm_send" );
        settings.setValue( "target", QString( "ipc" ) );
        settings.setValue( "prefix", QString( "Lidar" ) );
        settings.setValue( "channel", QString( "LivoxData" ) );
        settings.endGroup();
    }

    {
        settings.beginGroup( "main_params" );
        settings.setValue( "pid_path", QString( "/tmp/niias" ) );
        settings.setValue( "need_trace", bool( true ) );
        settings.endGroup();
    }

    {
        settings.beginGroup( "offset" );
        settings.setValue( "roll",  qreal( 0.0 ) );
        settings.setValue( "pitch", qreal( 0.0 ) );
        settings.setValue( "yaw",   qreal( 0.0 ) );
        settings.setValue( "x",     int(0) );
        settings.setValue( "y",     int(0) );
        settings.setValue( "z",     int(0) );
        settings.endGroup();
    }

    {
        settings.beginGroup( "lidar_params" );
        settings.setValue( "weather_suppress", bool( false ) );
        settings.setValue( "point_type", int(1) );
        settings.endGroup();
    }

    settings.sync();
}
//=======================================================================================

//=======================================================================================
QHostAddress Config::ip() const
{
    return _ip;
}
//=======================================================================================
void Config::ip( const QHostAddress& ip )
{
    _ip = ip;
}
//=======================================================================================

//=======================================================================================
bool Config::contains( const QString& broadcast )
{
    return _broadcast_list.contains( broadcast );
}
//=======================================================================================

//=======================================================================================
void Config::_make_default_path()
{
    _default_path = "../" + QCoreApplication::applicationName() +
                    "/cfg/" + QCoreApplication::applicationName() + ".ini";
}
//=======================================================================================
