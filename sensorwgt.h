#ifndef SENSORWGT_H
#define SENSORWGT_H

#include <QWidget>
#include "globaldefine.h"
#include <QTimer>
#include <QLabel>


namespace Ui {
class SensorWgt;
}

class SensorWgt : public QWidget
{
    Q_OBJECT

public:
    explicit SensorWgt(QWidget *parent = 0);
    ~SensorWgt();
private:
    void initParam();
    void initConnect();
    QString StatusToString(EnumSensorStatus status);
    void setLEDStatusByUTSensorStatus(EnumUTSensorType sensorType,EnumSensorStatus status);
    void setLEDStatusByPreSensorStatus(EnumPreSensorType sensorType,EnumSensorStatus status);
    EnumStatusLED SensorStatusToLEDStatus(EnumSensorStatus status);
    void mySleep(int msec);
private slots:
    void slUpdateUTSensorValue(EnumUTSensorType sensorType,quint16 utValue);
    void slUpdateUTSensorStatus(EnumUTSensorType sensorType,EnumSensorStatus status);
    void slUpdatePressureValue(EnumPreSensorType sensorType,quint16 preValue);
    void slUpdatePressureStatus(EnumPreSensorType sensorType,EnumSensorStatus status);
    void slChangedLEDStatus_UTSen1();
    void slChangedLEDStatus_UTSen2();
    void slChangedLEDStatus_PreSen1();
    void on_EnableUTSensor1_clicked(bool checked);

    void on_EnableUTSensor2_clicked(bool checked);

    void on_WarningValueUTSensor1_editingFinished();
    void on_WarningValueUTSensor2_editingFinished();

    void on_checkBox_clicked(bool checked);

    void on_WarningValuePreSensor1_editingFinished();

    void on_refreshUTSenParamBtn_clicked();

    void on_refreshPreSenParamBtn_clicked();

private:
    Ui::SensorWgt *ui;
    QTimer *m_TMflicker;
    EnumSensorStatus m_curStatusUTSenA,m_curStatusUTSenB,m_curStatusPreSenA;
    EnumStatusLED m_curLEDStatusUTSenA,m_curLEDStatusUTSenB,m_curLEDStatusPreSenA;
};

#endif // SENSORWGT_H
