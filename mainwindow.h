#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <configparmeter.h>
#include <ui_qstartwgt.h>
#include <QApplication>
#include "crawlerserial.h"
#include "configdlg.h"
#include "modbusmaster.h"
#include "crawlerstatusparam.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    bool keyPressHandler(QKeyEvent *keyValue, QObject *obj, QEvent *event);
    bool keyReleaseHandler(QKeyEvent *keyValue,QObject *obj, QEvent *event);
    void multiKeyPressProcess(Qt::Key lastKeyPress, Qt::Key currentReleasekey);
    void initConnect();
private slots:
    void on_actioncarmera_triggered();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    Ui::MainWindow *ui;
    QList<StruSerialInfo> m_listSerialDevice;
    QList<StruCameraCntCfg> m_listCameraDevice;
    QTimer *TMServoHorUp,*TMServoHorDown,*TMServoVerUp,*TMServoVerDown,*TMChangedSpeed;
    QList<Qt::Key> listKeyPress;
    configParmeter *m_setting;
    CrawlerSerial *m_crawlerSerial;
    ConfigDlg* m_configDlg;
    ModbusMaster* m_pMDmaster;
    CrawlerStatusParam* m_pCrawlerStatusParam;
};

#endif // MAINWINDOW_H
