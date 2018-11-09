#include "cameraloginsettingwgt.h"
#include "ui_cameraloginsettingwgt.h"

cameraLoginSettingWgt::cameraLoginSettingWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cameraLoginSettingWgt)
{
    ui->setupUi(this);
}

cameraLoginSettingWgt::~cameraLoginSettingWgt()
{
    delete ui;
}
