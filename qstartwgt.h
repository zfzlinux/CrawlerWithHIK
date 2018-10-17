#ifndef QSTARTWGT_H
#define QSTARTWGT_H

#include <QWidget>

namespace Ui {
class qStartWgt;
}

class qStartWgt : public QWidget
{
    Q_OBJECT

public:
    explicit qStartWgt(QWidget *parent = 0);
    ~qStartWgt();

private:
    Ui::qStartWgt *ui;
};

#endif // QSTARTWGT_H
