#include "configdlg.h"
#include "ui_configdlg.h"


ConfigDlg::ConfigDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDlg)
{
    ui->setupUi(this);
    configParmeter *setting = configParmeter::getInstance();
    m_ListCameraCfg = setting->getCameraCntConfigInfo();
    connect(ui->CameraWgt,SIGNAL(sgUpdateCameraCntConfig(StruCameraCntCfg *)),this,SLOT(slUpdateCameraCntConfig(StruCameraCntCfg *)));
    connect(ui->OtherParam,SIGNAL(sgEnableHeart(bool)),this,SIGNAL(sgEnableHeart(bool)));
    connect(ui->OtherParam,SIGNAL(sgGetModbusReg()),this,SIGNAL(sgReadModbusReg()));
}

ConfigDlg::~ConfigDlg()
{
    delete ui;
}

void ConfigDlg::slUpdateCameraCntConfig(StruCameraCntCfg *config)
{
    //update config
    m_updateCamerCfg = *config;
    if(m_ListCameraCfg.size()>0)
    {
        m_ListCameraCfg.replace(0,m_updateCamerCfg);
    }else
    {
        m_ListCameraCfg.append(m_updateCamerCfg);
    }
}

void ConfigDlg::on_OKBtn_clicked()
{
    //save parmeter
    configParmeter *setting = configParmeter::getInstance();
    //save camer config
    if(m_ListCameraCfg.size()>0)
    {
        setting->writeCameraCntConfigInfo(m_ListCameraCfg);
    }

    ui->SerialWgt->saveSetting();

    this->close();
}

void ConfigDlg::on_CancelBtn_clicked()
{
    //do noting
    this->close();
}
