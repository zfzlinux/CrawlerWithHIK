#include "configdlg.h"
#include "ui_configdlg.h"

ConfigDlg::ConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDlg)
{
    ui->setupUi(this);
}

ConfigDlg::~ConfigDlg()
{
    delete ui;
}
