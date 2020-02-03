#include "livoxcontainer.h"

//=======================================================================================
LivoxContainer::LivoxContainer( const QString& broadcast, QObject* parent )
    : QObject ( parent )
{
    _scatter = new CustomScatter( 1000, broadcast );

    _data_timer = new QTimer( this );
    _data_timer->start( data_ms );

    connect( _data_timer, &QTimer::timeout,
             [this]
    {
        _scatter->plot_pnts( convert<LivoxSpherPoint, LivoxRawPoint>( _pnts ) );
        emit transmit_packet_pnts( _pnts );
        _pnts.clear();
    });
}
//=======================================================================================

//=======================================================================================
void LivoxContainer::add_data( const QList<LivoxSpherPoint>& pnts )
{
    _pnts.append( pnts );
}
//=======================================================================================
