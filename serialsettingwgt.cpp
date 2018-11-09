#include "serialsettingwgt.h"
#include "ui_serialsettingwgt.h"

SerialSettingWgt::SerialSettingWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialSettingWgt)
{
    ui->setupUi(this);
}

SerialSettingWgt::~SerialSettingWgt()
{
    delete ui;
}
