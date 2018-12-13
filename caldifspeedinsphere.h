#ifndef CALDIFSPEEDINSPHERE_H
#define CALDIFSPEEDINSPHERE_H

#include <QWidget>

namespace Ui {
class CalDifSpeedInSphere;
}

class CalDifSpeedInSphere : public QWidget
{
    Q_OBJECT

public:
    explicit CalDifSpeedInSphere(QWidget *parent = 0);
    ~CalDifSpeedInSphere();

private:
    Ui::CalDifSpeedInSphere *ui;
};

#endif // CALDIFSPEEDINSPHERE_H
