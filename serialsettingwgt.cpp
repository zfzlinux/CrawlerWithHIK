#include "serialsettingwgt.h"
#include "ui_serialsettingwgt.h"

#include "configparmeter.h"
#include <QDebug>

SerialSettingWgt::SerialSettingWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialSettingWgt)
{
    ui->setupUi(this);
    this->addPortNameToCombox();
}

SerialSettingWgt::~SerialSettingWgt()
{
    delete ui;
}

void SerialSettingWgt::saveSetting()
{
    m_curSerialInfo.PortName = ui->PortNameComboBox->currentText();
    m_curSerialInfo.m_BaudRate = (StruSerialInfo::BaudRate)ui->BaudRateComboBox->currentText().toInt();
    m_curSerialInfo.m_DataBits = (StruSerialInfo::DataBits)ui->DataBitsComboBox->currentText().toInt();
    m_curSerialInfo.m_StopBits = (StruSerialInfo::StopBits)ui->StopBitsComboBox->currentText().toInt();
    m_curSerialInfo.m_Parity = (StruSerialInfo::Parity)ui->ParityComboBox->currentText().toInt();
    m_curSerialInfo.m_FlowControl = (StruSerialInfo::FlowControl)ui->FlowControlComboBox->currentText().toInt();

    if(m_serialPortlist.size()>0)
    {
        m_serialPortlist.replace(0,m_curSerialInfo);
    }else
    {
        m_serialPortlist.insert(0,m_curSerialInfo);
    }

    m_setting = configParmeter::getInstance();
    m_setting->writeSerialInfo(m_serialPortlist);
}

void SerialSettingWgt::addPortNameToCombox()
{
    //read Availd
    foreach (const QSerialPortInfo &qspinfo, QSerialPortInfo::availablePorts()) {
        QString sPortName = qspinfo.portName();
        ui->PortNameComboBox->addItem(sPortName);
    }
}

void SerialSettingWgt::on_PortNameComboBox_currentIndexChanged(const QString &portName)
{
   // emit this->sgPortNameChanged();
    m_setting = configParmeter::getInstance();
    m_serialPortlist = m_setting->getSerialInfo();

    foreach (const StruSerialInfo &serial, m_serialPortlist) {
        if(serial.PortName == portName)
        {
            QString strBuf = QString::number((quint32)serial.m_BaudRate);
            ui->BaudRateComboBox->setCurrentText(strBuf);
            strBuf = serial.m_DataBits;
            ui->DataBitsComboBox->setCurrentText(strBuf);
            strBuf = serial.m_Parity;
            ui->ParityComboBox->setCurrentText(strBuf);
            strBuf =serial.m_StopBits;
            ui->StopBitsComboBox->setCurrentText(strBuf);
            strBuf =serial.m_FlowControl;
            ui->FlowControlComboBox->setCurrentText(strBuf);
            return ;
        }
    }
}
