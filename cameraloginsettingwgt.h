#ifndef CAMERALOGINSETTINGWGT_H
#define CAMERALOGINSETTINGWGT_H

#include <QWidget>

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
    Ui::cameraLoginSettingWgt *ui;
};

#endif // CAMERALOGINSETTINGWGT_H
