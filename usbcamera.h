#ifndef USBCAMERA_H
#define USBCAMERA_H

#include <QObject>

class UsbCamera : public QObject
{
    Q_OBJECT
public:
    explicit UsbCamera(QObject *parent = nullptr);

signals:

public slots:
};

#endif // USBCAMERA_H