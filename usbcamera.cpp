#include "usbcamera.h"

UsbCamera::UsbCamera(QObject *parent) : QObject(parent)
{

}

QStringList UsbCamera::scanValidUsbCamera()
{
    QStringList validDevice;
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach(const QCameraInfo &cameraInfo,cameras)
    {
       QString strCameraName;
       strCameraName = QString("%1").arg(cameraInfo.description());
       validDevice.append(strCameraName);
    }
    return validDevice;
}

bool UsbCamera::openCameraWithIndex(quint16 index)
{
    m_cvUsbCamera = cvCreateCameraCapture(index);

    if(m_cvUsbCamera !=NULL)
    {
        return true;
    }else
    {
        return false;
    }
}
