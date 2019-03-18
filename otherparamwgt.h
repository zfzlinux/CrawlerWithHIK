#ifndef OTHERPARAMWGT_H
#define OTHERPARAMWGT_H

#include <QWidget>

namespace Ui {
class OtherParamWgt;
}

class OtherParamWgt : public QWidget
{
    Q_OBJECT

public:
    explicit OtherParamWgt(QWidget *parent = 0);
    ~OtherParamWgt();

private slots:

    void on_EnableHeartCheckBox_clicked(bool checked);
    void on_pushButton_clicked();

signals:
    void sgEnableHeart(bool enble);
    void sgGetModbusReg();

private:
    Ui::OtherParamWgt *ui;
};

#endif // OTHERPARAMWGT_H
