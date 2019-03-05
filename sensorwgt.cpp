#include "sensorwgt.h"
#include "ui_sensorwgt.h"
#include "crawlerstatusparam.h"
#include "crawlerserial.h"
#include <QDateTime>

SensorWgt::SensorWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorWgt),
    m_curLEDStatusUTSenA(RedLED),
    m_curLEDStatusUTSenB(RedLED),
    m_curLEDStatusPreSenA(RedLED)
{
    ui->setupUi(this);
    initParam();
    initConnect();
    m_TMflicker = new QTimer(this);
    m_TMflicker->setInterval(500);
    m_TMflicker->start();
    connect(m_TMflicker,SIGNAL(timeout()),this,SLOT(slChangedLEDStatus_UTSen1()));
    connect(m_TMflicker,SIGNAL(timeout()),this,SLOT(slChangedLEDStatus_UTSen2()));
    connect(m_TMflicker,SIGNAL(timeout()),this,SLOT(slChangedLEDStatus_PreSen1()));
}

SensorWgt::~SensorWgt()
{
    delete ui;
}

void SensorWgt::initParam()
{
    ui->ShowStatusLEDUTSensor1->setPixmap(QPixmap(":/image/resource/redShape.png"));
    ui->ShowStatusLEDUTSensor2->setPixmap(QPixmap(":/image/resource/redShape.png"));
    ui->ShowStatusLEDPreSensor1->setPixmap(QPixmap(":/image/resource/redShape.png"));
    //
    CrawlerStatusParam *crawlerStatusParam = CrawlerStatusParam::getInstance();
    this->slUpdateUTSensorValue(UT_SensorA,crawlerStatusParam->getCurUTSensorValue(UT_SensorA));
    this->slUpdateUTSensorStatus(UT_SensorA,crawlerStatusParam->getCurUTSensorStatus(UT_SensorA));
    this->slUpdateUTSensorValue(UT_SensorB,crawlerStatusParam->getCurUTSensorValue(UT_SensorB));
    this->slUpdateUTSensorStatus(UT_SensorB,crawlerStatusParam->getCurUTSensorStatus(UT_SensorB));
    this->slUpdatePressureValue(Pre_SensorA,crawlerStatusParam->getCurPressureSensorValue(Pre_SensorA));
    this->slUpdatePressureStatus(Pre_SensorA,crawlerStatusParam->getCurPreSenStatus(Pre_SensorA));

}

void SensorWgt::initConnect()
{
    CrawlerStatusParam *crawlerStatusParam = CrawlerStatusParam::getInstance();
    connect(crawlerStatusParam,SIGNAL(sgUpdateUTSensorValue(EnumUTSensorType,quint16)),this,SLOT(slUpdateUTSensorValue(EnumUTSensorType,quint16)));
    connect(crawlerStatusParam,SIGNAL(sgUpdateUTSensorStatus(EnumUTSensorType,EnumSensorStatus)),this,SLOT(slUpdateUTSensorStatus(EnumUTSensorType,EnumSensorStatus)));
    connect(crawlerStatusParam,SIGNAL(sgUpdatePressureValue(EnumPreSensorType,quint16)),this,SLOT(slUpdatePressureValue(EnumPreSensorType,quint16)));
    connect(crawlerStatusParam,SIGNAL(sgUpdatePressureStatus(EnumPreSensorType,EnumSensorStatus)),this,SLOT(slUpdatePressureStatus(EnumPreSensorType,EnumSensorStatus)));
}

QString SensorWgt::StatusToString(EnumSensorStatus status)
{
    QString strStatus;
    switch (status) {
    case Sen_DisConnect:
        strStatus = "Sen_DisConnect";
        break;
    case Sen_InConnect:
        strStatus = "Sen_InConnect";
        break;
    case Sen_SafeValue:
        strStatus = "Sen_SaftValue";
        break;
    case Sen_WarningValue:
        strStatus = "Sen_WarningValue";
        break;
    case Sen_Error:
        strStatus = "Sen_Error";
        break;
    default:
        break;
    }
    return strStatus;
}

void SensorWgt::setLEDStatusByUTSensorStatus(EnumUTSensorType sensorType, EnumSensorStatus status)
{
    switch (sensorType) {
    case UT_SensorA:
        m_curLEDStatusUTSenA = SensorStatusToLEDStatus(status);
        break;
    case UT_SensorB:
        m_curLEDStatusUTSenB = SensorStatusToLEDStatus(status);
        break;
    default:
        break;
    }
}

void SensorWgt::setLEDStatusByPreSensorStatus(EnumPreSensorType sensorType, EnumSensorStatus status)
{
    switch (sensorType) {
    case Pre_SensorA:
        m_curLEDStatusPreSenA = SensorStatusToLEDStatus(status);
        break;
    case Pre_SensorB:
        break;
    default:
        break;
    }
}

EnumStatusLED SensorWgt::SensorStatusToLEDStatus(EnumSensorStatus status)
{
    EnumStatusLED ledStatus;
    switch (status) {
    case Sen_DisConnect:
        ledStatus = RedLED;
        break;
    case Sen_InConnect:
        ledStatus = GreenLED;
        break;
    case Sen_SafeValue:
        ledStatus = GreenFlicker;
        break;
    case Sen_WarningValue:
        ledStatus = RedFlicker;
        break;
    default:
        ledStatus = RedLED;
        break;
    }
    return ledStatus;
}

void SensorWgt::mySleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}


void SensorWgt::slUpdateUTSensorValue(EnumUTSensorType sensorType, quint16 utValue)
{
    //update UT sensor value;
    switch (sensorType) {
    case UT_SensorA:
        ui->showCurDistance1->setText(QString::number(utValue));
        break;
    case UT_SensorB:
        ui->showCurDistance2->setText(QString::number(utValue));
        break;
    default:
        break;
    }
}

void SensorWgt::slUpdateUTSensorStatus(EnumUTSensorType sensorType, EnumSensorStatus status)
{
    switch (sensorType) {
    case UT_SensorA:
        ui->showUTStatus1->setText(StatusToString(status));

        break;
    case UT_SensorB:
        ui->showUTStatus2->setText(StatusToString(status));
        break;
    default:
        break;
    }
    setLEDStatusByUTSensorStatus(sensorType,status);
}

void SensorWgt::slUpdatePressureValue(EnumPreSensorType sensorType, quint16 preValue)
{
    switch (sensorType) {
    case Pre_SensorA:
        ui->showCurPressure1->setText(QString::number(preValue));
        break;
    case Pre_SensorB:
        break;
    default:
        break;
    }
}

void SensorWgt::slUpdatePressureStatus(EnumPreSensorType sensorType, EnumSensorStatus status)
{
    switch (sensorType) {
    case Pre_SensorA:
        ui->showPreStatus1->setText(StatusToString(status));
        break;
    case Pre_SensorB:
        break;
    default:
        break;
    }
    this->setLEDStatusByPreSensorStatus(sensorType,status);
}

void SensorWgt::slChangedLEDStatus_UTSen1()
{
    static bool isRedFlicker = false;
    static bool isGreenFlicker = false;
    switch (m_curLEDStatusUTSenA) {
    case RedLED:
        ui->ShowStatusLEDUTSensor1->setPixmap(QPixmap(":/image/resource/redShape.png"));
        break;
    case RedFlicker:
        if(isRedFlicker)
        {
            ui->ShowStatusLEDUTSensor1->setPixmap(QPixmap(":/image/resource/redShape.png"));
        }else
        {
            ui->ShowStatusLEDUTSensor1->clear();
        }
        isRedFlicker = !isRedFlicker;
        break;
    case GreenLED:
        ui->ShowStatusLEDUTSensor1->setPixmap(QPixmap(":/image/resource/greenShape.png"));
        break;
    case GreenFlicker:
        if(isGreenFlicker)
        {
            ui->ShowStatusLEDUTSensor1->setPixmap(QPixmap(":/image/resource/greenShape.png"));
        }else
        {
            ui->ShowStatusLEDUTSensor1->clear();
        }
        isGreenFlicker = !isGreenFlicker;
        break;
    default:
        break;
    }
}

void SensorWgt::slChangedLEDStatus_UTSen2()
{
    static bool isRedFlicker = false;
    static bool isGreenFlicker = false;
    switch (m_curLEDStatusUTSenB) {
    case RedLED:
        ui->ShowStatusLEDUTSensor2->setPixmap(QPixmap(":/image/resource/redShape.png"));
        break;
    case RedFlicker:
        if(isRedFlicker)
        {
            ui->ShowStatusLEDUTSensor2->setPixmap(QPixmap(":/image/resource/redShape.png"));
        }else
        {
            ui->ShowStatusLEDUTSensor2->clear();
        }
        isRedFlicker = !isRedFlicker;
        break;
    case GreenLED:
        ui->ShowStatusLEDUTSensor2->setPixmap(QPixmap(":/image/resource/greenShape.png"));
        break;
    case GreenFlicker:
        if(isGreenFlicker)
        {
            ui->ShowStatusLEDUTSensor2->setPixmap(QPixmap(":/image/resource/greenShape.png"));
        }else
        {
            ui->ShowStatusLEDUTSensor2->clear();
        }
        isGreenFlicker = !isGreenFlicker;
        break;
    default:
        break;
    }
}

void SensorWgt::slChangedLEDStatus_PreSen1()
{
    static bool isRedFlicker = false;
    static bool isGreenFlicker = false;
    switch (m_curLEDStatusPreSenA) {
    case RedLED:
        ui->ShowStatusLEDPreSensor1->setPixmap(QPixmap(":/image/resource/redShape.png"));
        break;
    case RedFlicker:
        if(isRedFlicker)
        {
            ui->ShowStatusLEDPreSensor1->setPixmap(QPixmap(":/image/resource/redShape.png"));
        }else
        {
            ui->ShowStatusLEDPreSensor1->clear();
        }
        isRedFlicker = !isRedFlicker;
        break;
    case GreenLED:
        ui->ShowStatusLEDPreSensor1->setPixmap(QPixmap(":/image/resource/greenShape.png"));
        break;
    case GreenFlicker:
        if(isGreenFlicker)
        {
            ui->ShowStatusLEDPreSensor1->setPixmap(QPixmap(":/image/resource/greenShape.png"));
        }else
        {
            ui->ShowStatusLEDPreSensor1->clear();
        }
        isGreenFlicker = !isGreenFlicker;
        break;
    default:
        break;
    }
}


void SensorWgt::on_EnableUTSensor1_clicked(bool checked)
{
    //
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    crawlerSerial->enableUTSensor(UT_SensorA,checked);
}

void SensorWgt::on_EnableUTSensor2_clicked(bool checked)
{
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    crawlerSerial->enableUTSensor(UT_SensorB,checked);
}

void SensorWgt::on_WarningValueUTSensor1_editingFinished()
{
    quint16 warnValue = ui->WarningValueUTSensor1->value();
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    crawlerSerial->setUTSensorWarningValue(UT_SensorA,warnValue);
}
void SensorWgt::on_WarningValueUTSensor2_editingFinished()
{
    quint16 warnValue = ui->WarningValueUTSensor2->value();
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    crawlerSerial->setUTSensorWarningValue(UT_SensorB,warnValue);
}

void SensorWgt::on_checkBox_clicked(bool checked)
{
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    crawlerSerial->enablePreSensor(Pre_SensorA,checked);
}

void SensorWgt::on_WarningValuePreSensor1_editingFinished()
{
    quint16 warnValue = ui->WarningValuePreSensor1->value();
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    crawlerSerial->setPreSensorWarningValue(Pre_SensorA,warnValue);
}

void SensorWgt::on_refreshUTSenParamBtn_clicked()
{
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    crawlerSerial->askCrawlerState(askCrawler_valueUTSensorA);
    this->mySleep(5);
    crawlerSerial->askCrawlerState(askCrawler_valueUTSensorB);
    this->mySleep(5);
    crawlerSerial->askCrawlerState(askCrawler_UTSensorStatus);
}

void SensorWgt::on_refreshPreSenParamBtn_clicked()
{
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    crawlerSerial->askCrawlerState(askCrawler_valuePressureA);
    this->mySleep(5);
    crawlerSerial->askCrawlerState(askCrawler_PressureSensorStatus);
    this->mySleep(5);
}
