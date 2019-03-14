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

private:
    Ui::OtherParamWgt *ui;
};

#endif // OTHERPARAMWGT_H
