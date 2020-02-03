#ifndef CONVERTER_H
#define CONVERTER_H

#include "livox_def.h"

#include <QList>
#include <QtMath>

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

        res.push_back( tmp );
    }

    return res;
}

#endif // CONVERTER_H
