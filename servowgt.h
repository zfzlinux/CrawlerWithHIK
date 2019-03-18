#ifndef SERVOWGT_H
#define SERVOWGT_H

#include <QWidget>
#include "crawlerserial.h"
#include "crawlerstatusparam.h"

namespace Ui {
class ServoWgt;
}

class ServoWgt : public QWidget
{
    Q_OBJECT

public:
    explicit ServoWgt(QWidget *parent = 0);
    ~ServoWgt();
    quint8 getHorizontalServoAngle();
    void setHorizontalServoAngle(quint8 angle);

    void HorizontalServoAngleUP();
    void HorizontalServoAngleDown();
    void VerticalServoAngleUP();
    void VerticalServoAngleDown();
signals:
    void sgUpdateAngleVer(quint8 angle);
    void sgUpdateAngleHor(quint8 angle);

private slots:
    void slHorizontalServoAngleUP();
    void slHorizontalServoAngleDown();
    void slVerticalServoAngleUP();
    void slVerticalServoAngleDown();
    void slUpdateAngleVer(quint8 angle);
    void slUpdateAngleHor(quint8 angle);
    void slLiftShaftStates(EnumLiftShaftMotorType MotorType,EnumLiftShaftStatus status);
private slots:
    void on_servohorizontalScrollBar_valueChanged(int value);

    void on_servoVerticalScrollBar_valueChanged(int value);
private:
    Ui::ServoWgt *ui;
    CrawlerSerial *m_crawlerSerial;
    CrawlerStatusParam *m_crawlerStatusParam;
};

#endif // SERVOWGT_H
