#include "cameraloginsettingwgt.h"
#include "ui_cameraloginsettingwgt.h"
#include <QDir>
#include <QFileDialog>
#include "configparmeter.h"

cameraLoginSettingWgt::cameraLoginSettingWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cameraLoginSettingWgt)
{
    ui->setupUi(this);

    //init
    this->initUIParmeter();
    m_cameraCntCfg.IPAddress = ui->ipAddressEdit->text();
    m_cameraCntCfg.UserName = ui->UserNameEdit->text();
    m_cameraCntCfg.Password = ui->PassWordEdit->text();
    m_cameraCntCfg.port = ui->PortSpinBox->value();
}

cameraLoginSettingWgt::~cameraLoginSettingWgt()
{
    delete ui;
}

void cameraLoginSettingWgt::initUIParmeter()
{
    configParmeter *setting = configParmeter::getInstance();
    setting->initSettings();
    QList<StruCameraCntCfg> m_listCameraCfg;
    m_listCameraCfg = setting->getCameraCntConfigInfo();
    if(m_listCameraCfg.size()>0)
    {
        ui->ipAddressEdit->setText(m_listCameraCfg.at(0).IPAddress);
        ui->UserNameEdit->setText(m_listCameraCfg.at(0).UserName);
        ui->PassWordEdit->setText(m_listCameraCfg.at(0).Password);
        ui->PortSpinBox->setValue(m_listCameraCfg.at(0).port);
        m_cameraCntCfg = m_listCameraCfg.at(0);
    }

    // save path
    QString savePath;
    savePath = setting->getSavePicturePath();
    ui->savePicturePathComboBox->clear();
    ui->savePicturePathComboBox->addItem(savePath);
    savePath = setting->getSaveVideoPath();
    ui->saveVideoPathcomboBox->clear();
    ui->saveVideoPathcomboBox->addItem(savePath);
}

void cameraLoginSettingWgt::on_ipAddressEdit_editingFinished()
{
    m_cameraCntCfg.IPAddress = ui->ipAddressEdit->text();
    emit sgUpdateCameraCntConfig(&m_cameraCntCfg);
}

void cameraLoginSettingWgt::on_UserNameEdit_editingFinished()
{
    m_cameraCntCfg.UserName = ui->UserNameEdit->text();
    emit sgUpdateCameraCntConfig(&m_cameraCntCfg);
}

void cameraLoginSettingWgt::on_PassWordEdit_editingFinished()
{
    m_cameraCntCfg.Password = ui->PassWordEdit->text();
    emit sgUpdateCameraCntConfig(&m_cameraCntCfg);
}

void cameraLoginSettingWgt::on_PortSpinBox_editingFinished()
{
    m_cameraCntCfg.port = ui->PortSpinBox->value();
    emit sgUpdateCameraCntConfig(&m_cameraCntCfg);
}

void cameraLoginSettingWgt::on_savePicturePathToolBtn_clicked()
{
    QString directory =QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("save path"),QDir::currentPath()));
    if(!directory.isEmpty())
    {
        if(ui->savePicturePathComboBox->findText(directory) == -1)
        {
            ui->savePicturePathComboBox->addItem(directory);
        }
        ui->savePicturePathComboBox->setCurrentIndex(ui->savePicturePathComboBox->findText(directory));
        configParmeter *setting = configParmeter::getInstance();
        setting->writeSavePicturePath(directory);
    }
}


void cameraLoginSettingWgt::on_saveVideoPathtoolButton_clicked()
{
    QString directory =QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("save path"),QDir::currentPath()));
    if(!directory.isEmpty())
    {
        if(ui->saveVideoPathcomboBox->findText(directory) == -1)
        {
            ui->saveVideoPathcomboBox->addItem(directory);
        }
        ui->saveVideoPathcomboBox->setCurrentIndex(ui->saveVideoPathcomboBox->findText(directory));
        configParmeter *setting = configParmeter::getInstance();
        setting->writeSaveVideoPath(directory);
    }
}
