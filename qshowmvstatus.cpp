#include "qshowmvstatus.h"
#include "ui_qshowmvstatus.h"
#include "crawlerstatusparam.h"
#include "crawlerserial.h"
#include <QDebug>

int qShowMvStatus::m_index =0;

qShowMvStatus::qShowMvStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qShowMvStatus),
    MaxModeNum(3),
    m_ledStatus(RedLED)
{
    ui->setupUi(this);
    this->changedUiPassModeIndex(m_index);
    this->setCurActionModePassIndex(m_index);

    initTheMap();
//    ui->pushButton->installEventFilter(this);
//    ui->pushButton_2->installEventFilter(this);
//    ui->pushButton_3->installEventFilter(this);
//    ui->pushButton_4->installEventFilter(this);
//    ui->pushButton_5->installEventFilter(this);
//    ui->pushButton_6->installEventFilter(this);

    initParam();
}

qShowMvStatus::~qShowMvStatus()
{
    delete ui;
}

void qShowMvStatus::initTheMap()
{
    m_keyToBtnMap.insert(KEY_FORWARD,ui->gofw_Btn);
    m_keyToBtnMap.insert(KEY_BACKWARD,ui->gobk_btn);
    m_keyToBtnMap.insert(KEY_GOLEFT,ui->golf_btn);
    m_keyToBtnMap.insert(KEY_GORIGHT,ui->gort_btn);
    m_keyToBtnMap.insert(KEY_SPEED_UP,ui->speedUp_btn);
    m_keyToBtnMap.insert(KEY_SPEED_DOWN,ui->speedDown_btn);
}

void qShowMvStatus::setGoForwardStyleSheet(bool clicked)
{
    if(clicked)
    {
        ui->gofw_Btn->setStyleSheet(QLatin1String("font: 75 9pt \"Agency FB\";\n""background-color: rgb(255, 0, 0);\n" "font: 14pt \"Agency FB\";"));
    }else
    {
        ui->gofw_Btn->setStyleSheet(QLatin1String("font: 75 9pt \"Agency FB\";\n" "font: 14pt \"Agency FB\";"));
    }
}

void qShowMvStatus::setBtnStyleSheet(QPushButton *obj, bool isPressed)
{
    if(isPressed)
    {
        obj->setStyleSheet(QLatin1String("font: 75 9pt \"Agency FB\";\n""background-color: rgb(255, 0, 0);\n" "font: 14pt \"Agency FB\";"));
        if(obj == ui->gofw_Btn || obj == ui->gobk_btn)
        {
            this->setCurLEDStatus(RedFlicker);
        }
    }else
    {
        obj->setStyleSheet(QLatin1String("font: 75 9pt \"Agency FB\";\n" "font: 14pt \"Agency FB\";"));
        //this->clearCurrentSpeed();
        this->setCurLEDStatus(RedLED);
        this->slUpdateLEDStatus();
    }
}

void qShowMvStatus::changedUiPassModeIndex(int modeIndex)
{
    switch (modeIndex) {
    case 0: // speed mode
        //
        ui->ShowCurMode->setText("Speed");
        //hide distance ui and differential speed ui
        this->showDistanceModeUI(false);
        this->showDifferentialSpeedModeUI(false);
        break;
    case 1: // distance mode
        ui->ShowCurMode->setText("Distance");
        this->showDistanceModeUI(true);
        this->showDifferentialSpeedModeUI(false);
        break;
    case 2: // difference mode
        ui->ShowCurMode->setText("Differential");
        this->showDistanceModeUI(false);
        this->showDifferentialSpeedModeUI(true);
        break;
    default:
        break;
    }
}

void qShowMvStatus::showSpeedModeUI(bool isShow)
{
    if(isShow)
    {

    }
    else{

    }
}

void qShowMvStatus::showDistanceModeUI(bool isShow)
{
    if(isShow)
    {
        ui->MaxMovelabel->show();
        ui->MaxMoveValueSpinBox->show();
    }
    else{
        ui->MaxMovelabel->hide();
        ui->MaxMoveValueSpinBox->hide();
    }
}

void qShowMvStatus::showDifferentialSpeedModeUI(bool isShow)
{
    if(isShow)
    {
        ui->LeftPWMlabel->show();
        ui->RightPWMlabel->show();
        ui->LeftPWMValueSpinBox->show();
        ui->RightPWMValueSpinBox->show();
    }
    else{
        ui->LeftPWMlabel->hide();
        ui->RightPWMlabel->hide();
        ui->LeftPWMValueSpinBox->hide();
        ui->RightPWMValueSpinBox->hide();
    }
}

void qShowMvStatus::setCurActionModePassIndex(int modeIndex)
{
    EnumActionMode curMode;
    switch (modeIndex) {
    case 0: // speed mode
        curMode  = speedMode;
        break;
    case 1: // distance mode
        curMode  = distanceMode;
        break;
    case 2: // difference mode
        curMode  = difSpeedMode;
        break;
    default:
        curMode  = speedMode;
        break;
    }

    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    CrawlerStatus->setActionMode(curMode);

    ModbusMaster* pMDmaster = ModbusMaster::getInstance();
    pMDmaster->setActionMode(curMode);

}

void qShowMvStatus::clearCurrentSpeed()
{
    ui->showCurSpeed->setText("0.00 mm/s");
}

void qShowMvStatus::setCurLEDStatus(EnumStatusLED status)
{
    this->m_ledStatus = status;
}

void qShowMvStatus::setBtnStyleSheet(Qt::Key keyValue, bool isPressed)
{
    if(!m_keyToBtnMap.contains(keyValue)) return;
    QPushButton *pushBtn = m_keyToBtnMap.value(keyValue);
    this->setBtnStyleSheet(pushBtn,isPressed);
}

void qShowMvStatus::initParam()
{
    quint8  curSpeedIndex;
    quint16 PWMValueLeft,PWMValueRight;
    pMDmaster = ModbusMaster::getInstance();
    m_crawlerStatus = CrawlerStatusParam::getInstance();
    connect(m_crawlerStatus,SIGNAL(sgUpdateCrawlerSpeedIndex(quint8)),this,SLOT(slUpdateSpeedIndex(quint8)));
    connect(m_crawlerStatus,SIGNAL(sgUpdatePWMValueByConfig(EnumPWMType,quint16)),this,SLOT(slUpdatePWMValueByConfig(EnumPWMType,quint16)));
    connect(m_crawlerStatus,SIGNAL(sgUpdateCurSpeed(double)),this,SLOT(slUpdateCurSpeed(double)));
    connect(m_crawlerStatus,SIGNAL(sgUpdateCurDistanceStatus(EnumDispModeStatus)),this,SLOT(slUpdateCurDistanceStatus(EnumDispModeStatus)));

    curSpeedIndex = m_crawlerStatus->getCurCrawlerSpeedIndex();
    this->slUpdateSpeedIndex(curSpeedIndex);
    PWMValueLeft= m_crawlerStatus->getCurPWMValueByConfig(PWMLeftByConfig);
    PWMValueRight = m_crawlerStatus->getCurPWMValueByConfig(PWMRightByConfig);

    ui->LeftPWMValueSpinBox->setValue(PWMValueLeft);
    ui->RightPWMValueSpinBox->setValue(PWMValueRight);

    this->slUpdateCurSpeed(0.00);

    this->m_TMLEDStatus = new QTimer(this);
    m_TMLEDStatus->setInterval(500);
    m_TMLEDStatus->start();
    connect(m_TMLEDStatus,SIGNAL(timeout()),this,SLOT(slUpdateLEDStatus()));

}

//void qShowMvStatus::slGoForward(bool clicked)
//{
//    if(clicked)
//    {
//        ui->gofw_Btn->setStyleSheet(QLatin1String("font: 75 9pt \"Agency FB\";\n""background-color: rgb(255, 0, 0);\n" "font: 14pt \"Agency FB\";"));
//    }else
//    {
//        ui->gofw_Btn->setStyleSheet(QLatin1String("font: 75 9pt \"Agency FB\";\n" "font: 14pt \"Agency FB\";"));
//    }
//}

void qShowMvStatus::slUpdateSpeedIndex(quint8 speedIndex)
{
    //quint8 speedIndex= m_crawlerStatus->getCurCrawlerSpeedIndex();
    QString strStall;
    strStall.append("CurStall:");
    strStall.append(QString::number(speedIndex));
    ui->CurrentStallLabel->setText(strStall);
}

void qShowMvStatus::slUpdatePWMValueByConfig(EnumPWMType PWMType, quint16 value)
{
    switch (PWMType) {
    case PWMLeftByConfig:
        ui->LeftPWMValueSpinBox->setValue(value);
        break;
    case PWMRightByConfig:
        ui->RightPWMValueSpinBox->setValue(value);
        break;
    default:
        break;
    }
}

void qShowMvStatus::slUpdateCurSpeed(double speed)
{
    QString strSpeed;
    strSpeed = QString::number(speed,'f',2);
    strSpeed.append("mm/s");
    ui->showCurSpeed->setText(strSpeed);
}

void qShowMvStatus::slUpdateCurDistanceStatus(EnumDispModeStatus status)
{
    m_curDispStatus = status;
    switch (status) {
    case Disp_MoveStop:
        this->setCurLEDStatus(RedLED);
        break;
    case Disp_MoveForwrad:
    case Disp_MoveBackwrad:
        this->setCurLEDStatus(RedFlicker);
        break;
    case Disp_FinishMove:
        this->setCurLEDStatus(GreenLED);
        break;
    default:
        break;
    }
}

void qShowMvStatus::on_ChangedModeBtn_clicked()
{
    m_index = (m_index +1) % MaxModeNum;
    this->changedUiPassModeIndex(m_index);
    this->setCurActionModePassIndex(m_index);
}

void qShowMvStatus::on_LeftPWMValueSpinBox_editingFinished()
{
    quint16 PWMValue =ui->LeftPWMValueSpinBox->value();

//    if(ENABLE_PRIVATE_SERIAL)
//    {
//        CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
//        crawlerSerial->setPWMValue(PWMLeftByConfig,PWMValue);
//    }
//    if(ENABLE_MODBUS)
//    {
//        pMDmaster->setPWMValueByConfig(PWMLeftByConfig,PWMValue);
//    }
    ui->LeftPWMValueSpinBox->clearFocus();
    emit this->sgUpdatePWMValueByConfig(PWMLeftByConfig,PWMValue);
}

void qShowMvStatus::on_RightPWMValueSpinBox_editingFinished()
{
    quint16 PWMValue =ui->RightPWMValueSpinBox->value();
//    if(ENABLE_PRIVATE_SERIAL)
//    {
//        CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
//        crawlerSerial->setPWMValue(PWMRightByConfig,PWMValue);
//    }
//    if(ENABLE_MODBUS)
//    {
//        pMDmaster->setPWMValueByConfig(PWMRightByConfig,PWMValue);
//    }
    ui->RightPWMValueSpinBox->clearFocus();
    emit this->sgUpdatePWMValueByConfig(PWMRightByConfig,PWMValue);
}

void qShowMvStatus::on_MaxMoveValueSpinBox_editingFinished()
{
    quint16 MaxMoveDistance = ui->MaxMoveValueSpinBox->value();
//    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
//    crawlerSerial->setMaxDistanceMove(MaxMoveDistance);
    ui->MaxMoveValueSpinBox->clearFocus();
    emit this->sgUpdateMaxMoveValue(MaxMoveDistance);
}

void qShowMvStatus::slUpdateLEDStatus()
{
    static bool isRedFlicker = false;
    static bool isGreenFlicker = false;
    switch (m_ledStatus) {
    case RedLED:
        ui->ShowLEDStatus->setPixmap(QPixmap(":/image/resource/redShape.png"));
        break;
    case RedFlicker:
        if(isRedFlicker)
        {
            ui->ShowLEDStatus->setPixmap(QPixmap(":/image/resource/redShape.png"));
        }else
        {
            ui->ShowLEDStatus->clear();
        }
        isRedFlicker = !isRedFlicker;
        break;
    case GreenLED:
        ui->ShowLEDStatus->setPixmap(QPixmap(":/image/resource/greenShape.png"));
        break;
    case GreenFlicker:
        if(isGreenFlicker)
        {
            ui->ShowLEDStatus->setPixmap(QPixmap(":/image/resource/greenShape.png"));
        }else
        {
            ui->ShowLEDStatus->clear();
        }
        isGreenFlicker = !isGreenFlicker;
        break;
    default:
        break;
    }
}
