#ifndef SERIALSETTINGWGT_H
#define SERIALSETTINGWGT_H

#include <QWidget>

namespace Ui {
class SerialSettingWgt;
}

class SerialSettingWgt : public QWidget
{
    Q_OBJECT

public:
    explicit SerialSettingWgt(QWidget *parent = 0);
    ~SerialSettingWgt();

private:
    Ui::SerialSettingWgt *ui;
};

#endif // SERIALSETTINGWGT_H
