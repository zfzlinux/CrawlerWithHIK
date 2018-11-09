#ifndef SERVOWGT_H
#define SERVOWGT_H

#include <QWidget>

namespace Ui {
class ServoWgt;
}

class ServoWgt : public QWidget
{
    Q_OBJECT

public:
    explicit ServoWgt(QWidget *parent = 0);
    ~ServoWgt();

private:
    Ui::ServoWgt *ui;
};

#endif // SERVOWGT_H
