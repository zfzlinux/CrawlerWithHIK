#ifndef QSTARTWGT_H
#define QSTARTWGT_H

#include <QWidget>
#include <QTimer>

#include "globaldefine.h"
#include "configparmeter.h"
#include "hikcamera.h"
#include "usbcamerathread.h"

namespace Ui {
class qStartWgt;
}

class qStartWgt : public QWidget
{
    Q_OBJECT

public:
    explicit qStartWgt(QWidget *parent = 0);
    ~qStartWgt();
public:
//    bool writerData(QByteArray &hexData);
//    bool writerData(QString strData);
    void setDisplayCameraHWND(HWND hwnd);
    void setDisplayLabel(QLabel* label);
private slots:
//    void slReadData();
private:
    void readConfigParam();
    void addVaildSerialToComBox();
    void addDefaultCameraInfoToComBox();

    void DealwithSerialDataReceive(QByteArray HexData);
    void recognitionCommand(QByteArray HexCommand);
    void dealFeedBackCMD(QByteArray &hexCMD);
    void askCrawlerStatus(EnumAskCrawlerStatus askStatus);

    void setSerialDevice(const StruSerialInfo &serialInfo);
    void setSerialDevice(const QString &portName);
    bool openSerialDevice(QString &portName);
    void changedUIAboutSerial(bool isOpened);

    bool openCameraDevice(QString &ipAddress);
    void changedUIAboutIPCamera(bool isOpened);
    void changedUIAboutUSBCamera(bool isOpened);
    void mySleep(int msec);
private slots:
    void slUpdateSerial();
    void slUpdateSerialInfo(StruSerialInfo &serialInfo);
    void slUpdateCameraInfo();
    void slChangedVideoStatusLabel();
    void slInitParamDelay();
    void slKeepHeartWithCrawler();
private slots:
    void on_OpenBtn_clicked();
    void on_restartBtn_clicked();    
    void on_SerialPortNameComBox_currentIndexChanged(const QString &portName);
    void on_CapturePictureBtn_clicked();

    void on_CaptureVideoBtn_toggled(bool checked);

    void on_USBCameraRadioButton_clicked(bool checked);

    void on_IPCameraRadioButton_clicked(bool checked);

private:
    Ui::qStartWgt *ui;
    QSerialPort *m_CrawlerSerial;
    QTimer *TMHeart;
    configParmeter *m_setting;
    HIKCamera *m_hikCamera;
    HWND m_displayHWND;
    QLabel* m_displayLabel;
    QList<StruSerialInfo> m_listSerialInfo;
    QList<StruCameraCntCfg> m_listCameraInfo;
    StruCameraCntCfg m_CameraCntCfg;
    bool m_isUpdateCamera;
    bool m_isSerialOpened,m_isCameraOpened;
    QTimer *TMCapVideo;
    QTimer *m_TMInit;
    UsbCameraThread* m_pUsbCameraThread;
public:
    static bool lastStatus_USB,lastStatus_IP;
};


#endif // QSTARTWGT_H
