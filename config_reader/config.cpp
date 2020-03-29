#include "config.h"

#include <QCoreApplication>
#include <QNetworkInterface>
#include <QVariant>

//=======================================================================================
Config::Config( const QString& fname )
    : _settings( new vsettings )
{
    _build();

    if ( !fname.isEmpty() )
    {
        _settings = new vsettings;
        _settings->from_ini_file( fname.toStdString() );
    }

    for ( const auto key: _settings->subgroup( receive.str ).keys() )
    {
        QString broadcast( _settings->subgroup( receive.str ).get( key ).c_str() );

        if ( !_settings->has_subgroup( broadcast.toStdString() ) )
            continue;

        auto group = _settings->subgroup( broadcast.toStdString() );

        _broadcast_list[ broadcast ].x = std::stoi( group.get( "x" ) );
        _broadcast_list[ broadcast ].y = std::stoi( group.get( "y" ) );
        _broadcast_list[ broadcast ].z = std::stoi( group.get( "z" ) );
        _broadcast_list[ broadcast ].roll  = std::stof( group.get( "roll" ) );
        _broadcast_list[ broadcast ].pitch = std::stof( group.get( "pitch" ) );
        _broadcast_list[ broadcast ].yaw   = std::stof( group.get( "yaw" ) );
    }

    {
        auto group = _settings->subgroup( zcm_send.str );

        zcm_send.target = QString( group.get( "target" ).c_str() );
        zcm_send.prefix = QString( group.get( "prefix" ).c_str() );
        zcm_send.data_channel = QString( group.get( "data_channel" ).c_str() );
        zcm_send.imu_channel = QString( group.get( "imu_channel" ).c_str() );
        zcm_send.info_channel = QString( group.get( "info_channel" ).c_str() );
    }

    {
        auto group = _settings->subgroup( main_params.str );

        main_params.ip = QString( group.get( "ip" ).c_str() );
        main_params.pid_path = QString( group.get( "pid_path" ).c_str() );
        main_params.data_freequency = QString( group.get( "data_freequency" ).c_str() ).toInt();
        main_params.imu_freequency = QString( group.get( "imu_freequency" ).c_str() ).toInt();
        main_params.info_freequency = QString( group.get( "info_freequency" ).c_str() ).toInt();
    }

    {
        auto group = _settings->subgroup( lidar_params.str );

        lidar_params.weather_suppress = QVariant( group.get( "weather_suppress" ).c_str() ).toBool();
    }

    ip( QHostAddress( main_params.ip ) );

    if ( ip().isNull() )
        for ( const QHostAddress& address: QNetworkInterface::allAddresses() )
            if ( address.protocol() == QAbstractSocket::IPv4Protocol &&
                 address != QHostAddress::LocalHost )
                ip( address );
}
//=======================================================================================
void Config::to_file( const QString& fname )
{
    _settings->to_ini_file( fname.toStdString() + ".cfg" );
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
QMap<QString, Config::Offset> &Config::broadcasts()
{
    return _broadcast_list;
}
//=======================================================================================


//=======================================================================================
bool Config::contains( const QString& broadcast )
{
    return _broadcast_list.contains( broadcast );
}
//=======================================================================================
void Config::deb()
{
    vdeb << _settings->to_ini();
}
//=======================================================================================


//=======================================================================================
void Config::_build()
{
    vsettings offset;
    {
        offset.set( "x", 0 );
        offset.set( "y", 0 );
        offset.set( "z", 0 );
        offset.set( "roll", 0.0 );
        offset.set( "pitch", 0.0 );
        offset.set( "yaw", 0.0 );
    }

    _settings->subgroup( this->receive.str ).set( "broadcast_1", "***************" );

    _settings->subgroup( this->zcm_send.str ).set( "target", "ipc" );
    _settings->subgroup( this->zcm_send.str ).set( "prefix", "Lidar" );
    _settings->subgroup( this->zcm_send.str ).set( "data_channel", "LivoxData" );
    _settings->subgroup( this->zcm_send.str ).set( "imu_channel", "LivoxIMU" );
    _settings->subgroup( this->zcm_send.str ).set( "info_channel", "LivoxInfo" );

    _settings->subgroup( this->main_params.str ).set( "ip", "192.168.***.***" );
    _settings->subgroup( this->main_params.str ).set( "pid_path", "/tmp/niias" );
    _settings->subgroup( this->main_params.str ).set( "data_freequency", 10 );
    _settings->subgroup( this->main_params.str ).set( "imu_freequency", 10 );
    _settings->subgroup( this->main_params.str ).set( "info_freequency", 1 );

    _settings->subgroup( this->lidar_params.str ).set( "weather_suppress", false );

    _settings->subgroup( "broadcast_1" ) = offset;
}
//=======================================================================================
