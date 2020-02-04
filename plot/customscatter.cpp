#include "customscatter.h"

using namespace QtDataVisualization;

//=======================================================================================
CustomScatter::CustomScatter( const int plot_time, const QString& name )
{
    this->setFlags( this->flags() ^ Qt::FramelessWindowHint );

    this->setPosition( { 900, 10 } );
    this->setWidth( 700 );
    this->setHeight( 700 );

    this->axisX()->setTitle("X");
    this->axisY()->setTitle("Y");
    this->axisZ()->setTitle("Z");

//    this->axisX()->setRange( - 5000, 5000 );
//    this->axisY()->setRange( 0, 30000 );
//    this->axisZ()->setRange( - 1000, 5000 );

    this->setShadowQuality( QAbstract3DGraph::ShadowQualityNone );

    auto preset = Q3DCamera::CameraPresetBehind;
    this->scene()->activeCamera()->setCameraPreset( ( Q3DCamera::CameraPreset ) preset );

    this->scene()->activeCamera()->setCameraPosition( - 2, 5, 250.0f );

    _layer = new QScatter3DSeries;

    addSeries( _layer );

    _layer->dataProxy()->addItems( _data );
    _layer->setBaseColor( qcolors.at( 1 ) );
    _layer->setItemSize( 0.05 );
    _layer->setMesh( QAbstract3DSeries::MeshPoint );

    _plot_timer = new QTimer( this );
    _plot_timer->start( plot_time );

    connect( _plot_timer, &QTimer::timeout, this, &CustomScatter::replot );
}
//=======================================================================================
CustomScatter::~CustomScatter()
{
    if ( _layer != nullptr )
        this->removeSeries( _layer );

    this->close();

    this->destroy();
}
//=======================================================================================


//=======================================================================================
#include <iostream>
void CustomScatter::plot_pnts( const QList<Pack>& packs )
{
    if ( packs.empty() )
        return;

    _data.clear();

    for ( const auto& pack: packs )
        for ( const auto& pnt: pack.pnts )
            _data << QVector3D( pnt.x, pnt.y, pnt.z );

    fill_layer( _data, *_layer );
}
//=======================================================================================


//=======================================================================================
void CustomScatter::replot()
{
    this->show();
}
//=======================================================================================
void CustomScatter::clear_layer( const int layer )
{
    if ( layer == 0 )
        this->removeSeries( _layer );
}
//=======================================================================================
void CustomScatter::clear_layers()
{
    this->removeSeries( _layer );
}
//=======================================================================================
void CustomScatter::clear_series()
{
    for ( auto& ser: this->seriesList() )
        this->removeSeries( ser );
}
//=======================================================================================
void CustomScatter::fill_layer( const QScatterDataArray& data, QScatter3DSeries& layer )
{
    layer.dataProxy()->removeItems( 0, layer.dataProxy()->itemCount() );
    layer.dataProxy()->addItems( data );
    layer.setItemSize( 0.005f );
    layer.setMesh( QAbstract3DSeries::MeshPoint );
}
//=======================================================================================
