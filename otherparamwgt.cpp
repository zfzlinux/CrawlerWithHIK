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
