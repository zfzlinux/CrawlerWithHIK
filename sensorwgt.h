#ifndef SENSORWGT_H
#define SENSORWGT_H

#include <QWidget>

namespace Ui {
class SensorWgt;
}

class SensorWgt : public QWidget
{
    Q_OBJECT

public:
    explicit SensorWgt(QWidget *parent = 0);
    ~SensorWgt();

private:
    Ui::SensorWgt *ui;
};

#endif // SENSORWGT_H
