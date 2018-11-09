#ifndef HIKCAMERA_H
#define HIKCAMERA_H

#include <QObject>

class HIKCamera : public QObject
{
    Q_OBJECT
public:
    explicit HIKCamera(QObject *parent = nullptr);

signals:

public slots:
};

#endif // HIKCAMERA_H