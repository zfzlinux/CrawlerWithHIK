#include "servowgt.h"
#include "ui_servowgt.h"
#include <QString>
#include <QDebug>
ServoWgt::ServoWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServoWgt)
{
    ui->setupUi(this);
    m_crawlerStatusParam = CrawlerStatusParam::getInstance();
    connect(m_crawlerStatusParam,SIGNAL(sgUpdateLiftShaftStatus(EnumLiftShaftMotorType ,EnumLiftShaftStatus)),this,SLOT(slLiftShaftStates(EnumLiftShaftMotorType,EnumLiftShaftStatus)));
    connect(m_crawlerStatusParam,SIGNAL(sgUpdateServoAngleVer(quint8)),this,SLOT(slUpdateAngleVer(quint8)));
    connect(m_crawlerStatusParam,SIGNAL(sgUpdateServoAngleHor(quint8)),this,SLOT(slUpdateAngleHor(quint8)));
}

ServoWgt::~ServoWgt()
{
    delete ui;
}

quint8 ServoWgt::getHorizontalServoAngle()
{
  return  ui->servohorizontalScrollBar->value();
}

void ServoWgt::setHorizontalServoAngle(quint8 angle)
{
    ui->servohorizontalScrollBar->setValue(angle);
}

void ServoWgt::HorizontalServoAngleUP()
{
    ui->servohorizontalScrollBar->setValue(ui->servohorizontalScrollBar->value()+1);
}

void ServoWgt::HorizontalServoAngleDown()
{
    ui->servohorizontalScrollBar->setValue(ui->servohorizontalScrollBar->value()-1);
}

void ServoWgt::VerticalServoAngleUP()
{
    ui->servoVerticalScrollBar->setValue(ui->servoVerticalScrollBar->value()+1);
}

void ServoWgt::VerticalServoAngleDown()
{
    ui->servoVerticalScrollBar->setValue(ui->servoVerticalScrollBar->value()-1);
}

void ServoWgt::slHorizontalServoAngleUP()
{
    ui->servohorizontalScrollBar->setValue(ui->servohorizontalScrollBar->value()+1);
    //qDebug()<<ui->servohorizontalScrollBar->value();
}

void ServoWgt::slHorizontalServoAngleDown()
{
    ui->servohorizontalScrollBar->setValue(ui->servohorizontalScrollBar->value()-1);
}

void ServoWgt::slVerticalServoAngleUP()
{
    ui->servoVerticalScrollBar->setValue(ui->servoVerticalScrollBar->value()+1);
    qDebug()<<ui->servoVerticalScrollBar->value();
}

void ServoWgt::slVerticalServoAngleDown()
{
    ui->servoVerticalScrollBar->setValue(ui->servoVerticalScrollBar->value()-1);
}

void ServoWgt::slUpdateAngleVer(quint8 angle)
{
    ui->servoVerticalScrollBar->setValue(angle);
}

void ServoWgt::slUpdateAngleHor(quint8 angle)
{
    ui->servohorizontalScrollBar->setValue(angle);
}

void ServoWgt::slLiftShaftStates(EnumLiftShaftMotorType MotorType, EnumLiftShaftStatus status)
{
    switch (MotorType)
    {
    case LS_VerMotor:
        switch (status) {
        case LS_MotorStop:
            ui->VerMotorStateLabel->setText(tr("stop"));
            break;
        case LS_MotorUp:
            ui->VerMotorStateLabel->setText(tr("moving to top"));
            break;
        case LS_MotorDown:
            ui->VerMotorStateLabel->setText(tr("moving to bottom"));
            break;
        case LS_MotorInTop:
            ui->VerMotorStateLabel->setText(tr("in top"));
            break;
        case LS_MotorInBottom:
            ui->VerMotorStateLabel->setText(tr("in bottom"));
            break;
        default:
            break;
        }
        break;
    case LS_HorMotor:
        switch (status) {
        case LS_MotorStop:
            ui->HorMotorStateLabel->setText(tr("stop"));
            break;
        case LS_MotorUp:
            ui->HorMotorStateLabel->setText(tr("moving to top"));
            break;
        case LS_MotorDown:
            ui->HorMotorStateLabel->setText(tr("moving to bottom"));
            break;
        case LS_MotorInTop:
            ui->HorMotorStateLabel->setText(tr("in top"));
            break;
        case LS_MotorInBottom:
            ui->HorMotorStateLabel->setText(tr("in bottom"));
            break;
        default:
            break;
        }
        break;
    }

}

void ServoWgt::on_servohorizontalScrollBar_valueChanged(int value)
{
    ui->HorizontalServoAngle->setText(QString::number(value));
    if(ENABLE_PRIVATE_SERIAL)
    {
        m_crawlerSerial = CrawlerSerial::getInstance();
        m_crawlerSerial->ChangedServoAngle_Hor(value);
    }
    emit this->sgUpdateAngleHor(value);

}

void ServoWgt::on_servoVerticalScrollBar_valueChanged(int value)
{
    ui->VerticalServoAngle->setText(QString::number(value));
    if(ENABLE_PRIVATE_SERIAL)
    {
        m_crawlerSerial = CrawlerSerial::getInstance();
        m_crawlerSerial->ChangedServoAngle_Ver(value);
    }
    emit this->sgUpdateAngleVer(value);
}



