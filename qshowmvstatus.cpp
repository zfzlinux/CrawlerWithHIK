#include "qshowmvstatus.h"
#include "ui_qshowmvstatus.h"

qShowMvStatus::qShowMvStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qShowMvStatus)
{
    ui->setupUi(this);
}

qShowMvStatus::~qShowMvStatus()
{
    delete ui;
}
