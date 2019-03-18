#include "otherparamwgt.h"
#include "ui_otherparamwgt.h"

OtherParamWgt::OtherParamWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OtherParamWgt)
{
    ui->setupUi(this);
}

OtherParamWgt::~OtherParamWgt()
{
    delete ui;
}

void OtherParamWgt::on_EnableHeartCheckBox_clicked(bool checked)
{

    emit this->sgEnableHeart(checked);
}

void OtherParamWgt::on_pushButton_clicked()
{
    emit this->sgGetModbusReg();
}
