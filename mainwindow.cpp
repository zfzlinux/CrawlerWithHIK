#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QEvent>
#include <QKeyEvent>
#include "crawlerserial.h"
#include "crawlerstatusparam.h"
#include <QProcess>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_configDlg(new ConfigDlg)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);

    this->installEventFilter(this);

    ui->startWgt->setDisplayCameraHWND((HWND)ui->displayImageLabel->winId());
    ui->startWgt->setDisplayLabel(ui->displayImageLabel);

    m_setting =configParmeter::getInstance();

    TMServoHorUp =new QTimer(this);
    TMServoHorDown = new QTimer(this);
    TMServoVerUp =new QTimer(this);
    TMServoVerDown = new QTimer(this);
    TMServoHorUp->stop();
    TMServoHorDown->stop();
    TMServoVerUp->stop();
    TMServoVerDown->stop();
    connect(TMServoHorUp,SIGNAL(timeout()),ui->servoWgt,SLOT(slHorizontalServoAngleUP()));
    connect(TMServoHorDown,SIGNAL(timeout()),ui->servoWgt,SLOT(slHorizontalServoAngleDown()));
    connect(TMServoVerUp,SIGNAL(timeout()),ui->servoWgt,SLOT(slVerticalServoAngleUP()));
    connect(TMServoVerDown,SIGNAL(timeout()),ui->servoWgt,SLOT(slVerticalServoAngleDown()));

    TMChangedSpeed = new QTimer();
    TMChangedSpeed->stop();

    QList<StruSerialInfo> m_list,m_rListSerial;

//    QSerialPort tst;
//    foreach(const QSerialPortInfo &qspinfo,QSerialPortInfo::availablePorts())
//    {
//          listSerialinfo.append(qspinfo);
//          tst.setPortName(qspinfo.portName());
//          tst.setBaudRate(QSerialPort::Baud9600);
//          m_serialinfo.PortName = tst.portName();
//          m_serialinfo.m_BaudRate =(StruSerialInfo::BaudRate)tst.baudRate();
//          m_serialinfo.m_DataBits =(StruSerialInfo::DataBits)tst.dataBits();
//          m_serialinfo.m_Parity =(StruSerialInfo::Parity)tst.parity();
//          m_serialinfo.m_StopBits =(StruSerialInfo::StopBits)tst.stopBits();
//          m_serialinfo.m_FlowControl =(StruSerialInfo::FlowControl)tst.flowControl();
//          m_list.append(m_serialinfo);
//    }

    //初始化
    configParmeter *m_configParmeter=configParmeter::getInstance();
    m_configParmeter->initSettings();
    //读取
    m_listSerialDevice = m_configParmeter->getSerialInfo();
    m_listCameraDevice = m_configParmeter->getCameraCntConfigInfo();

    if(m_listSerialDevice.size()>0)
    {
        //find
    }
    if(m_listCameraDevice.size()>0)
    {
        //
    }

    //
    m_crawlerSerial  = CrawlerSerial::getInstance();

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::keyPressHandler(QKeyEvent *keyValue, QObject *obj, QEvent *event)
{
    QString strCMD;
    switch (keyValue->key()) {
    case KEY_SEVOER_H_UP:
        if(!TMServoHorUp->isActive()) TMServoHorUp->start(200);
        //ui->servoWgt->slHorizontalServoAngleUP();
        break;
    case KEY_SEVOER_H_DOWN:
        if(!TMServoHorDown->isActive()) TMServoHorDown->start(200);
        //ui->servoWgt->slHorizontalServoAngleDown();
        break;
    case KEY_SEVOER_V_UP:
        if(!TMServoVerUp->isActive()) TMServoVerUp->start(200);
        //ui->servoWgt->slVerticalServoAngleUP();
        break;
    case KEY_SEVOER_V_DOWN:
        if(!TMServoVerDown->isActive()) TMServoVerDown->start(200);
        //ui->servoWgt->slVerticalServoAngleDown();
        break;
    case KEY_SPEED_UP:
        if (keyValue->isAutoRepeat()) return true;
        if(!TMChangedSpeed->isActive()) TMChangedSpeed->start(200);
        m_crawlerSerial->setSpeedUpByActionMode();
//        strCMD = m_setting->getMotorCMDByMode(em_speedUP);
//        m_crawlerSerial->writerData(strCMD);
        //ui->startWgt->writerData(strCMD);
        //display
        ui->showMVStatusWgt->setBtnStyleSheet(KEY_SPEED_UP,true);
        break;
    case KEY_SPEED_DOWN:
        if (keyValue->isAutoRepeat()) return true;
        if(!TMChangedSpeed->isActive()) TMChangedSpeed->start(200);
        m_crawlerSerial->setSpeedDownByActionMode();
//        strCMD = m_setting->getMotorCMDByMode(em_speedDown);
//        m_crawlerSerial->writerData(strCMD);
        //ui->startWgt->writerData(strCMD);
        //display
        ui->showMVStatusWgt->setBtnStyleSheet(KEY_SPEED_DOWN,true);
        break;
    case KEY_LIFTSHAFT_VUP:
        if (keyValue->isAutoRepeat()) return true;
        m_crawlerSerial->setupLiftShaftMotor(LS_VerMotor,LS_MotorUp);
        break;
    case KEY_LIFTSHAFT_VDOWN:
        if (keyValue->isAutoRepeat()) return true;
        m_crawlerSerial->setupLiftShaftMotor(LS_VerMotor,LS_MotorDown);
        break;
    case KEY_LIFTSHAFT_HUP:
        if (keyValue->isAutoRepeat()) return true;
        m_crawlerSerial->setupLiftShaftMotor(LS_HorMotor,LS_MotorUp);
        break;
    case KEY_LIFTSHAFT_HDOWN:
        if (keyValue->isAutoRepeat()) return true;
        m_crawlerSerial->setupLiftShaftMotor(LS_HorMotor,LS_MotorDown);
        break;
    case KEY_FORWARD:
        if (keyValue->isAutoRepeat()) return true;
        listKeyPress.removeAll(KEY_FORWARD);
        listKeyPress.append(KEY_FORWARD);
        //send cmdd pass mode
        strCMD = m_setting->getMotorCMDByMode(em_forworad);
        m_crawlerSerial->writerData(strCMD);
        //start send heart to crawler
        m_crawlerSerial->SendLoopHeartToCrawlerWith(true);
        //display
        ui->showMVStatusWgt->setBtnStyleSheet(KEY_FORWARD,true);
        break;
    case KEY_BACKWARD:
        if (keyValue->isAutoRepeat()) return true;
        listKeyPress.removeAll(KEY_BACKWARD);
        listKeyPress.append(KEY_BACKWARD);
        //send cmd pass mode
        strCMD = m_setting->getMotorCMDByMode(em_backward);
        m_crawlerSerial->writerData(strCMD);
//        ui->startWgt->writerData(strCMD);
        //start send heart to crawler
        m_crawlerSerial->SendLoopHeartToCrawlerWith(true);
        //display
        ui->showMVStatusWgt->setBtnStyleSheet(KEY_BACKWARD,true);
        break;
    case KEY_GOLEFT:
        if (keyValue->isAutoRepeat()) return true;
        listKeyPress.removeAll(KEY_GOLEFT);
        listKeyPress.append(KEY_GOLEFT);
        //send cmd pass mode
        strCMD = m_setting->getMotorCMDByMode(em_left);
        m_crawlerSerial->writerData(strCMD);
//        ui->startWgt->writerData(strCMD);
        //start send heart to crawler
        m_crawlerSerial->SendLoopHeartToCrawlerWith(true);
        //display
        ui->showMVStatusWgt->setBtnStyleSheet(KEY_GOLEFT,true);
        break;
    case KEY_GORIGHT:
        if (keyValue->isAutoRepeat()) return true;
        listKeyPress.removeAll(KEY_GORIGHT);
        listKeyPress.append(KEY_GORIGHT);
        strCMD = m_setting->getMotorCMDByMode(em_right);
        m_crawlerSerial->writerData(strCMD);
//        ui->startWgt->writerData(strCMD);
        //start send heart to crawler
        m_crawlerSerial->SendLoopHeartToCrawlerWith(true);
        //display
        ui->showMVStatusWgt->setBtnStyleSheet(KEY_GORIGHT,true);
        break;
    case KEY_FRONT_LEFT:
        if (keyValue->isAutoRepeat()) return true;
        listKeyPress.removeAll(KEY_FRONT_LEFT);
        listKeyPress.append(KEY_FRONT_LEFT);

        strCMD = m_setting->getMotorCMDByMode(em_frontLeft);
        m_crawlerSerial->writerData(strCMD);
//        ui->startWgt->writerData(strCMD);
        //start send heart to crawler
        m_crawlerSerial->SendLoopHeartToCrawlerWith(true);
        break;
    case KEY_FRONT_RIGHT:
        if (keyValue->isAutoRepeat()) return true;
        listKeyPress.removeAll(KEY_FRONT_RIGHT);
        listKeyPress.append(KEY_FRONT_RIGHT);

        strCMD = m_setting->getMotorCMDByMode(em_frontRight);
        m_crawlerSerial->writerData(strCMD);
//        ui->startWgt->writerData(strCMD);
        //start send heart to crawler
        m_crawlerSerial->SendLoopHeartToCrawlerWith(true);
        break;
    default:
        return QWidget::eventFilter(obj,event);
    }
    return true;
}

bool MainWindow::keyReleaseHandler(QKeyEvent *keyValue, QObject *obj, QEvent *event)
{
    QString strCMD;
    switch (keyValue->key()) {
    case KEY_SEVOER_H_UP:
        if(TMServoHorUp->isActive())TMServoHorUp->stop();
        break;
    case KEY_SEVOER_H_DOWN:
        if(TMServoHorDown->isActive()) TMServoHorDown->stop();
        break;
    case KEY_SEVOER_V_UP:
        if(TMServoVerUp->isActive()) TMServoVerUp->stop();
        break;
    case KEY_SEVOER_V_DOWN:
        if(TMServoVerDown->isActive())TMServoVerDown->stop();
        break;
    case KEY_SPEED_UP:
        TMChangedSpeed->stop();
        //display
        ui->showMVStatusWgt->setBtnStyleSheet(KEY_SPEED_UP,false);
        break;
    case KEY_SPEED_DOWN:
        TMChangedSpeed->stop();
        //display
        ui->showMVStatusWgt->setBtnStyleSheet(KEY_SPEED_DOWN,false);
        break;
    case KEY_LIFTSHAFT_VUP:
        m_crawlerSerial->setupLiftShaftMotor(LS_VerMotor,LS_MotorStop);
        break;
    case KEY_LIFTSHAFT_VDOWN:
        m_crawlerSerial->setupLiftShaftMotor(LS_VerMotor,LS_MotorStop);
        break;
    case KEY_LIFTSHAFT_HUP:
        m_crawlerSerial->setupLiftShaftMotor(LS_HorMotor,LS_MotorStop);
        break;
    case KEY_LIFTSHAFT_HDOWN:
        m_crawlerSerial->setupLiftShaftMotor(LS_HorMotor,LS_MotorStop);
        break;
    case KEY_FORWARD:
        if (keyValue->isAutoRepeat()) return true;
        listKeyPress.removeAll(KEY_FORWARD);
        if(listKeyPress.size()>0)
        {
            Qt::Key lastKeyValue = listKeyPress.last();
            multiKeyPressProcess(lastKeyValue,KEY_FORWARD);
            return true;
        }
        //send cmd with any key press
        strCMD = m_setting->getMotorCMDByMode(em_stop);
         m_crawlerSerial->writerData(strCMD);
//        ui->startWgt->writerData(strCMD);

         //start send heart to crawler
         m_crawlerSerial->SendLoopHeartToCrawlerWith(false);
        //display
        ui->showMVStatusWgt->setBtnStyleSheet(KEY_FORWARD,false);

        break;
    case KEY_BACKWARD:
        if (keyValue->isAutoRepeat()) return true;
        listKeyPress.removeAll(KEY_BACKWARD);
        if(listKeyPress.size()>0)
        {
            Qt::Key lastKeyValue = listKeyPress.last();
            multiKeyPressProcess(lastKeyValue,KEY_BACKWARD);
            return true;
        }
        //send cmd with any key press
        strCMD = m_setting->getMotorCMDByMode(em_stop);
         m_crawlerSerial->writerData(strCMD);
//        ui->startWgt->writerData(strCMD);
         //start send heart to crawler
         m_crawlerSerial->SendLoopHeartToCrawlerWith(false);
        //display
        ui->showMVStatusWgt->setBtnStyleSheet(KEY_BACKWARD,false);
        break;
    case KEY_GOLEFT:
        if (keyValue->isAutoRepeat()) return true;
        listKeyPress.removeAll(KEY_GOLEFT);
        if(listKeyPress.size()>0)
        {
            Qt::Key lastKeyValue = listKeyPress.last();
            multiKeyPressProcess(lastKeyValue,KEY_GOLEFT);
            return true;
        }
        //send cmd with any key press
        strCMD = m_setting->getMotorCMDByMode(em_stop);
        m_crawlerSerial->writerData(strCMD);
//        ui->startWgt->writerData(strCMD);
        //start send heart to crawler
        m_crawlerSerial->SendLoopHeartToCrawlerWith(false);
        //display
        ui->showMVStatusWgt->setBtnStyleSheet(KEY_GOLEFT,false);
        break;
    case KEY_GORIGHT:
        if (keyValue->isAutoRepeat()) return true;
        listKeyPress.removeAll(KEY_GORIGHT);
        if(listKeyPress.size()>0)
        {
            Qt::Key lastKeyValue = listKeyPress.last();
            multiKeyPressProcess(lastKeyValue,KEY_GORIGHT);
            return true;
        }
        //send cmd with any key press
        strCMD = m_setting->getMotorCMDByMode(em_stop);
         m_crawlerSerial->writerData(strCMD);
//        ui->startWgt->writerData(strCMD);
         //start send heart to crawler
         m_crawlerSerial->SendLoopHeartToCrawlerWith(false);
        //display
        ui->showMVStatusWgt->setBtnStyleSheet(KEY_GORIGHT,false);
        break;
    case KEY_FRONT_LEFT:
        if (keyValue->isAutoRepeat()) return true;
        listKeyPress.removeAll(KEY_FRONT_LEFT);
        if(listKeyPress.size()>0)
        {
            Qt::Key lastKeyValue = listKeyPress.last();
            multiKeyPressProcess(lastKeyValue,KEY_FRONT_LEFT);
            return true;
        }
        //send stop cmd;
        strCMD = m_setting->getMotorCMDByMode(em_stop);
         m_crawlerSerial->writerData(strCMD);
//        ui->startWgt->writerData(strCMD);
         //start send heart to crawler
         m_crawlerSerial->SendLoopHeartToCrawlerWith(false);
        break;
    case KEY_FRONT_RIGHT:
        if (keyValue->isAutoRepeat()) return true;
        listKeyPress.removeAll(KEY_FRONT_RIGHT);
        if(listKeyPress.size()>0)
        {
            Qt::Key lastKeyValue = listKeyPress.last();
            multiKeyPressProcess(lastKeyValue,KEY_FRONT_RIGHT);
            return true;
        }
        strCMD = m_setting->getMotorCMDByMode(em_stop);
         m_crawlerSerial->writerData(strCMD);
//        ui->startWgt->writerData(strCMD);
         //start send heart to crawler
         m_crawlerSerial->SendLoopHeartToCrawlerWith(false);
        break;
    default:
        return QWidget::eventFilter(obj,event);
    }
    return true;
}

void MainWindow::multiKeyPressProcess(Qt::Key lastKeyPress, Qt::Key currentReleasekey)
{
    //
    QString strCMD;
    strCMD = m_setting->getMotorCMDByModeQtKey(lastKeyPress);
    m_crawlerSerial->writerData(strCMD);
//    ui->startWgt->writerData(strCMD);
    //display
    ui->showMVStatusWgt->setBtnStyleSheet(currentReleasekey,false);
}

void MainWindow::on_actioncarmera_triggered()
{
    m_configDlg->show();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *KeyValue = static_cast<QKeyEvent *>(event);
        keyPressHandler(KeyValue,obj,event);
        return true;
    }
    else if(event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *KeyValue = static_cast<QKeyEvent *>(event);
        keyReleaseHandler(KeyValue,obj,event);
        return true;
    }
    else
    {
        return QWidget::eventFilter(obj,event);
    }

    return true;
}

