#ifndef LIVOXCONTAINER_H
#define LIVOXCONTAINER_H

#include "customscatter.h"
#include "defs.h"
#include "converter.h"

#include "livox_def.h"

#include <QObject>

//=======================================================================================
class LivoxContainer : public QObject
{
    Q_OBJECT

public:

    explicit LivoxContainer( const QString& broadcast = {}, QObject* parent = nullptr );

    //-----------------------------------------------------------------------------------

signals:
    void transmit_packet_pnts( const QList<LivoxSpherPoint>& );

public slots:

    void add_data( const QList<LivoxSpherPoint>& pnts );

    //-----------------------------------------------------------------------------------

private:

    CustomScatter *_scatter = nullptr;

    QTimer *_data_timer = nullptr;

    QList<LivoxSpherPoint> _pnts;

};
//=======================================================================================

#endif // LIVOXCONTAINER_H
