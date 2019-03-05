#ifndef USBCAMERA_H
#define USBCAMERA_H

#include <QObject>
#include <QList>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraInfo>
#include <QStringList>

#include "opencv/cv.h"
#include "opencv/highgui.h"

class UsbCamera : public QObject
{
    Q_OBJECT
public:
    explicit UsbCamera(QObject *parent = nullptr);
    QStringList scanValidUsbCamera();
    bool openCameraWithIndex(quint16 index);
signals:
public slots:

private:
    CvCapture* m_cvUsbCamera;
};


#endif // USBCAMERA_H
