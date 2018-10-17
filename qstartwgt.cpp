#include "qstartwgt.h"
#include "ui_qstartwgt.h"

qStartWgt::qStartWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qStartWgt)
{
    ui->setupUi(this);
}

qStartWgt::~qStartWgt()
{
    delete ui;
}
