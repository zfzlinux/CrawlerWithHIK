#ifndef QSHOWMVSTATUS_H
#define QSHOWMVSTATUS_H

#include <QWidget>
#include <QPushButton>
#include <QMap>
#include "globaldefine.h"
#include "crawlerstatusparam.h"
#include <QTimer>
namespace Ui {
class qShowMvStatus;
}

class qShowMvStatus : public QWidget
{
    Q_OBJECT

public:
    explicit qShowMvStatus(QWidget *parent = 0);
    ~qShowMvStatus();
public:
    void setBtnStyleSheet(Qt::Key keyValue, bool isPressed);
private:
    void initParam();
    void initTheMap();
    void setGoForwardStyleSheet(bool clicked);
    void setBtnStyleSheet(QPushButton *obj, bool isPressed);
    void changedUiPassModeIndex(int modeIndex);
    void showSpeedModeUI(bool isShow);
    void showDistanceModeUI(bool isShow);
    void showDifferentialSpeedModeUI(bool isShow);
    void setCurActionModePassIndex(int modeIndex);
    void clearCurrentSpeed();
    void setCurLEDStatus(EnumStatusLED status);
private slots:
    //void slGoForward(bool clicked);
    void slUpdateSpeedIndex(quint8 speedIndex);
    void slUpdatePWMValueByConfig(EnumPWMType PWMType,quint16 value);
    void slUpdateCurSpeed(double speed);
    void slUpdateCurDistanceStatus(EnumDispModeStatus status);
    void on_ChangedModeBtn_clicked();

    void on_LeftPWMValueSpinBox_editingFinished();

    void on_RightPWMValueSpinBox_editingFinished();

    void on_MaxMoveValueSpinBox_editingFinished();
    void slUpdateLEDStatus();
private:
    Ui::qShowMvStatus *ui;
    QMap<Qt::Key,QPushButton *> m_keyToBtnMap;
    CrawlerStatusParam *m_crawlerStatus;
    static int m_index;
    const int MaxModeNum;
    EnumDispModeStatus m_curDispStatus;
    EnumStatusLED m_ledStatus;
    QTimer *m_TMLEDStatus;
};

#endif // QSHOWMVSTATUS_H
