#ifndef CONFIGDLG_H
#define CONFIGDLG_H

#include <QDialog>
#include "globaldefine.h"
#include "configparmeter.h"
#include "cameraloginsettingwgt.h"
#include "serialsettingwgt.h"

namespace Ui {
class ConfigDlg;
}

class ConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDlg(QWidget *parent = 0);
    ~ConfigDlg();
public slots:
    void slUpdateCameraCntConfig(StruCameraCntCfg *config);
signals:
    void sgUpdateCameraCntConfig(const StruCameraCntCfg *config);

    void sgEnableHeart(bool enable);
    void sgReadModbusReg();
private slots:
    void on_OKBtn_clicked();

    void on_CancelBtn_clicked();

private:
    Ui::ConfigDlg *ui;
    QList<StruCameraCntCfg> m_ListCameraCfg;
    StruCameraCntCfg m_updateCamerCfg;
};

#endif // CONFIGDLG_H
