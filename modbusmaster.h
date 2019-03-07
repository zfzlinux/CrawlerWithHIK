#ifndef MODBUSMASTER_H
#define MODBUSMASTER_H

#include <QObject>

class ModbusMaster : public QObject
{
    Q_OBJECT
public:
    explicit ModbusMaster(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MODBUSMASTER_H