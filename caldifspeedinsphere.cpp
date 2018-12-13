#include "caldifspeedinsphere.h"
#include "ui_caldifspeedinsphere.h"

CalDifSpeedInSphere::CalDifSpeedInSphere(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalDifSpeedInSphere)
{
    ui->setupUi(this);
}

CalDifSpeedInSphere::~CalDifSpeedInSphere()
{
    delete ui;
}
