#include "servowgt.h"
#include "ui_servowgt.h"

ServoWgt::ServoWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServoWgt)
{
    ui->setupUi(this);
}

ServoWgt::~ServoWgt()
{
    delete ui;
}
