#ifndef LIVOXDRIVER_H
#define LIVOXDRIVER_H

#include <QObject>

class LivoxDriver : public QObject
{
    Q_OBJECT
public:
    explicit LivoxDriver(QObject *parent = nullptr);

signals:

public slots:
};

#endif // LIVOXDRIVER_H
