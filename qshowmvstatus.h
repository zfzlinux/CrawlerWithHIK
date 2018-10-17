#ifndef QSHOWMVSTATUS_H
#define QSHOWMVSTATUS_H

#include <QWidget>

namespace Ui {
class qShowMvStatus;
}

class qShowMvStatus : public QWidget
{
    Q_OBJECT

public:
    explicit qShowMvStatus(QWidget *parent = 0);
    ~qShowMvStatus();

private:
    Ui::qShowMvStatus *ui;
};

#endif // QSHOWMVSTATUS_H
