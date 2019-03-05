#ifndef SERIALSETTINGWGT_H
#define SERIALSETTINGWGT_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "configparmeter.h"

namespace Ui {
class SerialSettingWgt;
}

class SerialSettingWgt : public QWidget
{
    Q_OBJECT

public:
    explicit SerialSettingWgt(QWidget *parent = 0);
    ~SerialSettingWgt();

public:
    void saveSetting();
private slots:
    void on_PortNameComboBox_currentIndexChanged(const QString &portName);
signals:
    void sgPortNameChanged(QString portName);
private:
    void addPortNameToCombox();
private:
    Ui::SerialSettingWgt *ui;
    configParmeter *m_setting;
    QList<StruSerialInfo> m_serialPortlist;
    StruSerialInfo m_curSerialInfo;
};

#endif // SERIALSETTINGWGT_H
