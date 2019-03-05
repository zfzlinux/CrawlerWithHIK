#ifndef CAMERALOGINSETTINGWGT_H
#define CAMERALOGINSETTINGWGT_H

#include <QWidget>
#include "globaldefine.h"
#include "configparmeter.h"

namespace Ui {
class cameraLoginSettingWgt;
}

class cameraLoginSettingWgt : public QWidget
{
    Q_OBJECT
public:
    explicit cameraLoginSettingWgt(QWidget *parent = 0);
    ~cameraLoginSettingWgt();
private:
    void initUIParmeter();
signals:
    void sgUpdateCameraCntConfig(StruCameraCntCfg *config);

private slots:
    void on_ipAddressEdit_editingFinished();

    void on_UserNameEdit_editingFinished();

    void on_PassWordEdit_editingFinished();

    void on_PortSpinBox_editingFinished();

    void on_savePicturePathToolBtn_clicked();

    void on_saveVideoPathtoolButton_clicked();

private:
    Ui::cameraLoginSettingWgt *ui;
    StruCameraCntCfg m_cameraCntCfg;
};

#endif // CAMERALOGINSETTINGWGT_H
