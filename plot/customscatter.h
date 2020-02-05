#ifndef CUSTOMSCATTER_H
#define CUSTOMSCATTER_H

#include "defs.h"

#include <Q3DScatter>
#include <QWidget>
#include <QObject>
#include <QTimer>
#include <QList>

using namespace QtDataVisualization;

//=======================================================================================
class CustomScatter : public Q3DScatter
{
    Q_OBJECT

public:

    CustomScatter( const int plot_time, const QString& name = {} );

    ~CustomScatter();

    //-----------------------------------------------------------------------------------

public slots:

    void plot_pnts( const QList<Pack>& packs );

    //-----------------------------------------------------------------------------------

private:

    QScatter3DSeries* _layer;
    QScatterDataArray _data;

    QTimer *_plot_timer = nullptr;

    //-----------------------------------------------------------------------------------

    using LidarColors = QList<QColor>;

    LidarColors qcolors
    {
        QColor( Qt::black    ),
                QColor( Qt::blue   ),
                QColor( Qt::red    ),
                QColor( Qt::green )
    };

    //-----------------------------------------------------------------------------------

    void clear_layer( const int _layer );

    void clear_layers();

    void clear_series();

    void fill_layer( const QScatterDataArray& data, QScatter3DSeries& _layer );

private slots:

    void replot();

};

//=======================================================================================

#endif // CUSTOMSCATTER_H
