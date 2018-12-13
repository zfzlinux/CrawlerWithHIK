#include "sensorwgt.h"
#include "ui_sensorwgt.h"

SensorWgt::SensorWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorWgt)
{
    ui->setupUi(this);
}

SensorWgt::~SensorWgt()
{
    delete ui;
}
