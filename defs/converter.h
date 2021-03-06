#ifndef CONVERTER_H
#define CONVERTER_H

#include "livoxmessages.h"

#include "livox_def.h"

#include <QList>
#include <QtMath>

//=======================================================================================
// Point types conversion

template <typename TI, typename TO>
QList<TO> convert( const QList<TI>& pnts )
{
    QList<TO> res;

    for ( const auto& pnt: pnts )
    {
        TO tmp;

        // Single: Polar to Cartesian
        if ( std::is_same<TI, LivoxSpherPoint>::value &&
             std::is_same<TO, LivoxRawPoint>::value )
        {
            auto theta_rad = qDegreesToRadians( qreal( pnt.theta / 100. ) );
            auto phi_rad   = qDegreesToRadians( qreal( pnt.phi / 100. ) );

            tmp.x = int32_t( pnt.depth * qSin( theta_rad ) * qCos( phi_rad ) );
            tmp.y = int32_t( pnt.depth * qSin( theta_rad ) * qSin( phi_rad ) );
            tmp.z = int32_t( pnt.depth * qCos( theta_rad ) );

            tmp.reflectivity = pnt.reflectivity;
        }

        else if ( std::is_same<TI, LivoxSpherPoint>::value &&
                  std::is_same<TO, Point>::value )
        {
            auto theta_rad = qDegreesToRadians( qreal( pnt.theta / 100. ) );
            auto phi_rad   = qDegreesToRadians( qreal( pnt.phi / 100. ) );

            tmp.x = float( pnt.depth * qSin( theta_rad ) * qCos( phi_rad ) ) / 1000;
            tmp.y = float( pnt.depth * qSin( theta_rad ) * qSin( phi_rad ) ) / 1000;
            tmp.z = float( pnt.depth * qCos( theta_rad ) ) / 1000;

            tmp.reflectivity = pnt.reflectivity;

            tmp.tag = 0;
        }

        res.push_back( tmp );
    }

    return res;
}
//=======================================================================================
template <typename T>
Pack convert( const T& data )
{
    Pack res;

    res.version = data.version;
    res.slot_id = data.slot_id;
    res.lidar_id = data.lidar_id;
    res.timestamp_type = data.timestamp_type;
    res.data_type = data.data_type;
    res.timestamp = data.timestamp;

    res.pnts = convert<LivoxSpherPoint, Point>( data.pnts );

    return res;
}
//=======================================================================================

//=======================================================================================

#endif // CONVERTER_H
