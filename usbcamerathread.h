#ifndef USBCAMERATHREAD_H
#define USBCAMERATHREAD_H

#include <QObject>
#include <QThread>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QCameraInfo>
#include <QStringList>
#include <QLabel>

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/imgproc/imgproc.hpp"

typedef  struct  __IMAGE_DATA_{
    QImage    data;
    QMutex     mutex;
} ImageData;

class UsbCameraThread : public QThread
{
    Q_OBJECT
public:
    UsbCameraThread(QObject *parent = nullptr);
    ~UsbCameraThread();
    void setInterval(quint16 interval);
    QStringList findValidUsbCamera();
    bool openCameraWithIndex(quint16 index);
    void closeCamera();
    bool ReadFrameForUsbCamera(ImageData& imageBuf);
    void diplayWithImageData(QLabel* dispalyLabel);
    void setDisplayLabel(QLabel *displayLabel);
    void capturePicture(QString fileSavePath);
    void StartCaptureVideo(QString fileSavePath);
    void stopCaptureVideo();
protected:
    virtual void run();
private:
    bool m_isOpened;
    bool m_isCaptureVideo;
    quint16 m_interval;
    CvCapture* m_pCvUsbCamera;
    IplImage* m_pUsbImage;
    ImageData m_imageData;
    CvVideoWriter* m_videoWriter;
    QLabel* m_displayLabe;
};

#endif // USBCAMERATHREAD_H
