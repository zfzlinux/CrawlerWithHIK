#include "qstartwgt.h"
#include "ui_qstartwgt.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QFileDialog>
#include <QDateTime>
#include <QPixmap>
#include <QMessageBox>
#include "crawlerstatusparam.h"
#include "crawlerserial.h"
#include <QDebug>
#include "modbusmaster.h"

bool qStartWgt::lastStatus_USB;
bool qStartWgt::lastStatus_IP;

qStartWgt::qStartWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qStartWgt),
    m_CrawlerSerial(new QSerialPort),
    m_hikCamera(new HIKCamera),
    m_displayHWND(NULL),
    m_displayLabel(new QLabel()),
    m_isUpdateCamera(false),
    m_isSerialOpened(false),
    m_isCameraOpened(false),
    m_pUsbCameraThread(new UsbCameraThread())
{
    ui->setupUi(this);
    readConfigParam();
    addVaildSerialToComBox();
    addDefaultCameraInfoToComBox();
    TMHeart = new QTimer();
    TMHeart->setInterval(4000);
    TMHeart->stop();
    connect(TMHeart,SIGNAL(timeout()),this,SLOT(slKeepHeartWithCrawler()));
    //
    connect(m_setting,SIGNAL(sgUpdataSerialInfo()),this,SLOT(slUpdateSerial()));
    connect(m_setting,SIGNAL(sgUpdateCameraInfo()),this,SLOT(slUpdateCameraInfo()));

    TMCapVideo = new QTimer();
    TMCapVideo->setInterval(500);
    TMCapVideo->stop();

    connect(TMCapVideo,SIGNAL(timeout()),this,SLOT(slChangedVideoStatusLabel()));    

    m_TMInit = new QTimer();
    m_TMInit->setInterval(1000);
    m_TMInit->stop();
    connect(m_TMInit,SIGNAL(timeout()),this,SLOT(slInitParamDelay()));

    lastStatus_USB = ui->USBCameraRadioButton->isChecked();
    lastStatus_IP = ui->IPCameraRadioButton->isChecked();

    QStringList strValidDevice = m_pUsbCameraThread->findValidUsbCamera();
    ui->UsbCameraComboBox->clear();
    ui->UsbCameraComboBox->addItems(strValidDevice);

    ui->UsbCameraComboBox->hide();
    ui->USBCameraRadioButton->hide();
    ui->USBCameraStatusLabel->hide();

}

qStartWgt::~qStartWgt()
{
    m_pUsbCameraThread->closeCamera();
    m_pUsbCameraThread->exit();
    delete ui;
}

//void qStartWgt::slReadData()
//{
//    //qDebug("read data");
//    QByteArray readData = m_CrawlerSerial->readAll();
//    if(readData.isEmpty()) return ;
//    DealwithSerialDataReceive(readData);
//}

void qStartWgt::readConfigParam()
{
    m_setting = configParmeter::getInstance();

    m_listSerialInfo.clear();
    m_listSerialInfo = m_setting->getSerialInfo();

    m_listCameraInfo.clear();
    m_listCameraInfo = m_setting->getCameraCntConfigInfo();

}

void qStartWgt::addVaildSerialToComBox()
{
    ui->SerialPortNameComBox->clear();
    foreach (const QSerialPortInfo &qspinfo, QSerialPortInfo::availablePorts()) {
        QString sPortName = qspinfo.portName();
        ui->SerialPortNameComBox->addItem(sPortName);
        //将配置文件中的第一个串口信息设置为默认
        if(m_listSerialInfo.size()==0) return;
        if(sPortName == m_listSerialInfo.at(0).PortName)
        {
            ui->SerialPortNameComBox->setCurrentText(sPortName);
        }
    }
}

void qStartWgt::addDefaultCameraInfoToComBox()
{
    if(m_listCameraInfo.size()>0)
    {
        ui->CameraIPcomboBox->clear();
        foreach(const StruCameraCntCfg &cameraCntCfg,m_listCameraInfo)
        {
            ui->CameraIPcomboBox->addItem(cameraCntCfg.IPAddress);
        }
        m_CameraCntCfg = m_listCameraInfo.at(0);
        ui->CameraIPcomboBox->setCurrentText(m_listCameraInfo.at(0).IPAddress);
    }
}

void qStartWgt::DealwithSerialDataReceive(QByteArray HexData)
{
    //
    static QByteArray byPacket;
    QByteArray byCommand;
    byPacket.append(HexData);
    bool isHaveHead = byPacket.contains((unsigned char)ID_PacketHead);
    bool isHaveTail = byPacket.contains((unsigned char)ID_PacketTail);

    if( isHaveHead && isHaveTail)
    {
        int packsize,TailPos,HeadPos;
        TailPos = byPacket.indexOf((unsigned char)ID_PacketTail);
        HeadPos = byPacket.indexOf((unsigned char)ID_PacketHead);
        packsize = TailPos - HeadPos;
        if(packsize == PacketLength-1)
        {
            //
            byCommand = byPacket.mid(HeadPos+1,PacketLength-2);
            byPacket  = byPacket.mid(TailPos+1);

            qDebug("read packet");
            recognitionCommand(byCommand);
        }
    }
}

void qStartWgt::recognitionCommand(QByteArray HexCommand)
{
    unsigned char firstID;
    firstID = HexCommand.at(0);
    switch (firstID) {
        case ID_Feedback:
            dealFeedBackCMD(HexCommand);
            break;
        default:
            break;
    }
}

void qStartWgt::dealFeedBackCMD(QByteArray &hexCMD)
{

    configParmeter *setting = configParmeter::getInstance();
    StruSpeedModeParameter speedmode;
    speedmode = setting->getSpeedModeParam();

    quint8 ucStop,ucForward,ucBackword,ucLeft,ucRight;
    bool ok;
    ucStop= speedmode.stop.mid(3,2).toInt(&ok,16);
    ucForward = speedmode.up.mid(3,2).toInt(&ok,16);
    ucBackword = speedmode.down.mid(3,2).toInt(&ok,16);
    ucLeft = speedmode.left.mid(3,2).toInt(&ok,16);
    ucRight = speedmode.right.mid(3,2).toInt(&ok,16);

    //lift shaft
    Strulift_shaft liftShaft;
    liftShaft = setting->getLiftShaftParam();
    quint8 ucVMStop,ucVMUP,ucVMDown;
    ucVMStop = liftShaft.VerMotorStop.toInt(&ok,16);
    ucVMUP = liftShaft.VerMotorUp.toInt(&ok,16);
    ucVMDown = liftShaft.VerMotorDown.toInt(&ok,16);

    //servo
    StruServo_Angle servo;
    servo = setting->getServoParam();
    quint8 ucVerServo,ucHorServo;
    ucVerServo =servo.cmd_angle_V.toInt(&ok,16);
    ucHorServo =servo.cmd_angle_H.toInt(&ok,16);

    //
    CrawlerStatusParam *CrawlerStatus = CrawlerStatusParam::getInstance();
    switch (hexCMD.at(1)) {
    case ID_MotorMove:
        if(hexCMD.at(2) == ucStop)
        {
            CrawlerStatus->keepCurCrawlerMovingState(crawler_stop);
        }
        if(hexCMD.at(2)==ucForward)
        {
            CrawlerStatus->keepCurCrawlerMovingState(crawler_forward);
            qDebug("feedback fw");
        }
        if(hexCMD.at(2)==ucBackword)
        {
            CrawlerStatus->keepCurCrawlerMovingState(crawler_backward);
        }
        if(hexCMD.at(2)==ucLeft)
        {
            CrawlerStatus->keepCurCrawlerMovingState(crawler_trunleft);
        }
        if(hexCMD.at(2)==ucRight)
        {
            CrawlerStatus->keepCurCrawlerMovingState(crawler_trunright);
        }
        break;
    case ID_VerMotorMove:
        if(hexCMD.at(2)==ucVMStop)
        {

        }
        if(hexCMD.at(2)==ucVMUP)
        {

        }
        if(hexCMD.at(2)==ucVMDown)
        {

        }
        break;
    case ID_Servo:
        if(hexCMD.at(2)==ucVerServo)
        {

        }
        if(hexCMD.at(2)==ucHorServo)
        {

        }
        break;
    default:
        break;
    }
}

void qStartWgt::askCrawlerStatus(EnumAskCrawlerStatus askStatus)
{
    //m_CrawlerSerial->write();
    switch (askStatus) {
    case askCrawler_speed_all:
        //m_CrawlerSerial->write();
        break;
    default:
        break;
    }
}

void qStartWgt::setSerialDevice(const StruSerialInfo &serialInfo)
{
    m_CrawlerSerial->setPortName(serialInfo.PortName);
    m_CrawlerSerial->setBaudRate(serialInfo.m_BaudRate);
    m_CrawlerSerial->setStopBits((QSerialPort::StopBits)serialInfo.m_StopBits);
    m_CrawlerSerial->setDataBits((QSerialPort::DataBits)serialInfo.m_DataBits);
    m_CrawlerSerial->setParity((QSerialPort::Parity)serialInfo.m_Parity);
    m_CrawlerSerial->setFlowControl((QSerialPort::FlowControl)serialInfo.m_FlowControl);
}

void qStartWgt::setSerialDevice(const QString &portName)
{
    if(m_CrawlerSerial->isOpen()) m_CrawlerSerial->close();
    //if exist then read the save setting param;
    foreach (const StruSerialInfo &serialInfo, m_listSerialInfo) {
        if(serialInfo.PortName == portName)
        {
            this->setSerialDevice(serialInfo);
            return;
        }
    }
    //have not found then
    m_CrawlerSerial->setPortName(portName);
}

bool qStartWgt::openSerialDevice(QString &portName)
{
    this->setSerialDevice(portName);
    bool isOpened = false;
    if(ENABLE_PRIVATE_SERIAL)
    {
        CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
        isOpened = crawlerSerial->openCrawlerSerial(m_CrawlerSerial);
    }
    if(ENABLE_MODBUS)
    {
        ModbusMaster *pModbusMaster = ModbusMaster::getInstance();
        isOpened = pModbusMaster->openModbus(m_CrawlerSerial,1);
        //init modbus registers
        isOpened = pModbusMaster->initRegWithSlaveNum(1);
        //MODBUS_REG modbusReg = pModbusMaster->getModbusReg();
    }
    return isOpened;
}

void qStartWgt::changedUIAboutSerial(bool isOpened)
{
    if(!isOpened)
    {
        ui->SerialStatusLabel->setStyleSheet(QLatin1String("background-color: rgb(255,0, 0);\n" ));
    }else
    {
        ui->SerialStatusLabel->setStyleSheet(QLatin1String("background-color: rgb(0,255, 0);\n" ));
    }
}

bool qStartWgt::openCameraDevice(QString &ipAddress)
{
    bool isFound = false;
    foreach (const StruCameraCntCfg &cameraCntcfg, m_listCameraInfo) {
        if(cameraCntcfg.IPAddress == ipAddress)
        {
            m_CameraCntCfg = cameraCntcfg;
            isFound = true;
        }
    }
    //open camera
    if(isFound)
    {
        m_hikCamera->setCameraLoginInfo(m_CameraCntCfg);
        m_hikCamera->setDisplayWidgetHWND(m_displayHWND);
        isFound = m_hikCamera->openHIKCamera();
    }
    return isFound;
}

void qStartWgt::changedUIAboutIPCamera(bool isOpened)
{
    if(!isOpened)
    {
        ui->CameraStatusLabel->setStyleSheet(QLatin1String("background-color: rgb(255,0, 0);\n" ));
    }else
    {
        ui->CameraStatusLabel->setStyleSheet(QLatin1String("background-color: rgb(0,255, 0);\n" ));
    }
}

void qStartWgt::changedUIAboutUSBCamera(bool isOpened)
{
    if(!isOpened)
    {
        ui->USBCameraStatusLabel->setStyleSheet(QLatin1String("background-color: rgb(255,0, 0);\n" ));
    }else
    {
        ui->USBCameraStatusLabel->setStyleSheet(QLatin1String("background-color: rgb(0,255, 0);\n" ));
    }
}

void qStartWgt::mySleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void qStartWgt::slUpdateSerial()
{
    //qDebug()<<"update";
    readConfigParam();
    if(m_listSerialInfo.size()>0)
    {
        ui->SerialPortNameComBox->setCurrentText(m_listSerialInfo.at(0).PortName);
    }
    if(m_isSerialOpened)
    {
        m_CrawlerSerial->close();
        m_isSerialOpened =false;
        this->changedUIAboutSerial(m_isSerialOpened);
        ui->OpenBtn->setEnabled(true);
    }

}

void qStartWgt::slUpdateSerialInfo(StruSerialInfo &serialInfo)
{
    ui->SerialPortNameComBox->setCurrentText(serialInfo.PortName);
}

void qStartWgt::slUpdateCameraInfo()
{
    readConfigParam();
    if(m_listCameraInfo.size()>0)
    {
        ui->CameraIPcomboBox->clear();
        ui->CameraIPcomboBox->addItem(m_listCameraInfo.at(0).IPAddress);
        //ui->CameraIPcomboBox->setCurrentText(m_listCameraInfo.at(0).IPAddress);
    }
    //close something and reOpened;
    if(m_isCameraOpened)
    {
        m_hikCamera->closeHIKCamera();
        m_isCameraOpened =false;
        this->changedUIAboutIPCamera(m_isCameraOpened);
        ui->OpenBtn->setEnabled(true);
    }
}

void qStartWgt::slChangedVideoStatusLabel()
{
    static bool isShowed=false;
    if(isShowed)
    {
        ui->showCaptureStatusLabel->setPixmap(QPixmap(":/image/resource/red.png"));
    }else
    {
        ui->showCaptureStatusLabel->clear();
    }
    isShowed = !isShowed;
}

void qStartWgt::slInitParamDelay()
{
    m_TMInit->stop();
    //ask motion status
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    //心跳应该在车体运动的状态下才开启，其他指令不需要心跳保持强连接。
    //crawlerSerial->SendLoopHeartToCrawlerWith(true);

    crawlerSerial->askCrawlerState(askCrawler_motion);
    this->mySleep(50);
    crawlerSerial->askCrawlerState(askCrawler_speed_left);
    this->mySleep(50);


}

void qStartWgt::slKeepHeartWithCrawler()
{
    //send heart to crawler
    CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
    crawlerSerial->askCrawlerState(askCrawler_heart);
}

//bool qStartWgt::writerData(QByteArray &hexData)
//{
//    //
//    if(!m_CrawlerSerial->isOpen()) return false;
//    int len = m_CrawlerSerial->write(hexData);
//    if(len==0) return false;
//    return true;
//}

//bool qStartWgt::writerData(QString strData)
//{
//    if(!m_CrawlerSerial->isOpen()) return false;
//    if(strData.isEmpty()) return false;
//    QString strTemp;
//    configParmeter *setting = configParmeter::getInstance();
//    StruPacketParameter struPacket = setting->getPacketConfig();
//    strTemp.append(struPacket.head);
//    strTemp.append(strData);
//    strTemp.append(struPacket.tail);
//    strTemp =  strTemp.simplified();
//    QByteArray byData = QByteArray::fromHex(strTemp.toLatin1());
//    int len = m_CrawlerSerial->write(byData);
//    if(len == 0) return false;
//    return true;
//}

void qStartWgt::setDisplayCameraHWND(HWND hwnd)
{
    m_displayHWND = hwnd;
}

void qStartWgt::setDisplayLabel(QLabel *label)
{
    m_displayLabel = label;
}

void qStartWgt::on_OpenBtn_clicked()
{
   QString PortName = ui->SerialPortNameComBox->currentText();
   //Serial open with the portName
   m_isSerialOpened = this->openSerialDevice(PortName);
   this->changedUIAboutSerial(m_isSerialOpened);

   if(ui->IPCameraRadioButton->isChecked())
   {
       //get the cameraCntcfg;
       QString ipAddress = ui->CameraIPcomboBox->currentText();
       m_isCameraOpened = this->openCameraDevice(ipAddress);
       this->changedUIAboutIPCamera(m_isCameraOpened);
   }

   if(ui->USBCameraRadioButton->isChecked())
   {
       //open usb camera
       int index = ui->UsbCameraComboBox->currentIndex();
        bool isOpened = m_pUsbCameraThread->openCameraWithIndex(index);
        if(isOpened)
        {
           //
           m_pUsbCameraThread->setDisplayLabel(m_displayLabel);
           m_pUsbCameraThread->start();
        }
        this->changedUIAboutUSBCamera(isOpened);
   }

   ui->OpenBtn->setEnabled(false);
   m_TMInit->start();
   //this->TMHeart->start();

}

void qStartWgt::on_restartBtn_clicked()
{
    //serial
    if(ENABLE_MODBUS)
    {
        ModbusMaster* pMDmaster = ModbusMaster::getInstance();
        pMDmaster->closeModbus();
    }
    //
    if(ENABLE_PRIVATE_SERIAL)
    {
        CrawlerSerial *crawlerSerial = CrawlerSerial::getInstance();
        crawlerSerial->closeCrawlerSerial();
    }


    ui->OpenBtn->setEnabled(true);
    this->changedUIAboutSerial(false);

    if(ui->IPCameraRadioButton->isChecked())
    {
     m_hikCamera->closeHIKCamera();
     this->changedUIAboutIPCamera(false);
    }
    if(ui->USBCameraRadioButton->isChecked())
    {
        m_pUsbCameraThread->closeCamera();
        m_pUsbCameraThread->terminate();
        this->changedUIAboutUSBCamera(false);
    }
}

void qStartWgt::on_SerialPortNameComBox_currentIndexChanged(const QString &portName)
{
    this->setSerialDevice(portName);
}

void qStartWgt::on_CapturePictureBtn_clicked()
{
    QString filePath = m_setting->getSavePicturePath();
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString fileName = time.toString("yyMMdd_hhmmss"); //设置显示格式
    fileName.append(".bmp");
    filePath.append("\\");
    filePath.append(fileName);

    QString fileSavePath = QFileDialog::getSaveFileName(this,tr("Save File"),filePath,tr("Images (*.bmp)"));

    if(ui->IPCameraRadioButton->isChecked())
    {
        if(m_isCameraOpened)
        {
            m_hikCamera->capturePicture(fileSavePath);
        }
    }

    if(ui->USBCameraRadioButton->isChecked())
    {
        m_pUsbCameraThread->capturePicture(fileSavePath);
    }
}

void qStartWgt::on_CaptureVideoBtn_toggled(bool checked)
{
    static bool isSaveVideo = false;
    if(checked)
    {
        QString filePath = m_setting->getSaveVideoPath();
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString fileName = time.toString("yyMMdd_hhmmss"); //设置显示格式
        fileName.append(".avi");
        filePath.append("\\");
        filePath.append(fileName);

        QString fileSavePath = QFileDialog::getSaveFileName(this,tr("Save Video File"),filePath,tr("Video (*.mp4)"));
        if(m_isCameraOpened && (!fileSavePath.isEmpty()))
        {
            m_hikCamera->StartCaptureVideo(fileSavePath);
            TMCapVideo->start();
            isSaveVideo = true;
        }
//        //usbcamera
        if(ui->USBCameraRadioButton->isChecked() && (!fileSavePath.isEmpty()))
        {
            m_pUsbCameraThread->StartCaptureVideo(fileSavePath);
            TMCapVideo->start();
            isSaveVideo = true;
        }
    }
    else
    {
        TMCapVideo->stop();
        if(m_isCameraOpened && isSaveVideo)
        {
            m_hikCamera->StopCaptureVideo();
            m_hikCamera->closeHIKCamera();
            //get the cameraCntcfg;
            QString ipAddress = ui->CameraIPcomboBox->currentText();
            m_isCameraOpened = this->openCameraDevice(ipAddress);
            this->changedUIAboutIPCamera(m_isCameraOpened);
            QMessageBox msgbox;
            msgbox.setText("the video is save sucessful");
            msgbox.exec();
        }

        //usbcamera
        if(ui->USBCameraRadioButton->isChecked())
        {
            m_pUsbCameraThread->stopCaptureVideo();
            isSaveVideo = true;
        }

    }
}

void qStartWgt::on_USBCameraRadioButton_clicked(bool checked)
{
    if(checked != lastStatus_USB)
    {
        lastStatus_USB = checked;
        lastStatus_IP = !checked;
        if(checked)
        {
            ui->IPCameraRadioButton->setChecked(false);
            //close IP camera and ready to open usb camera
            m_hikCamera->closeHIKCamera();
            QStringList strValidDevice = m_pUsbCameraThread->findValidUsbCamera();
            ui->UsbCameraComboBox->clear();
            ui->UsbCameraComboBox->addItems(strValidDevice);
        }else{
            ui->IPCameraRadioButton->setChecked(true);
        }
    }
}

void qStartWgt::on_IPCameraRadioButton_clicked(bool checked)
{
    if(checked != lastStatus_IP)
    {
        lastStatus_USB = !checked;
        lastStatus_IP = checked;
        if(checked)
        {
            ui->USBCameraRadioButton->setChecked(false);
            //close usb camera and ready to open IP camera
        }else{
            ui->USBCameraRadioButton->setChecked(true);
        }
    }

}
