#include "usbcamerathread.h"
#include "QDebug"

UsbCameraThread::UsbCameraThread(QObject *parent)
    :QThread(parent),
      m_interval(100),
      m_isCaptureVideo(false),
      m_pCvUsbCamera(NULL),
      m_pUsbImage(NULL),
      m_videoWriter(NULL),
      m_displayLabe(new QLabel)
{

}

UsbCameraThread::~UsbCameraThread()
{
    closeCamera();
    this->terminate();
}

void UsbCameraThread::run()
{
    while(true){
        //qDebug()<<"thead runing";
        msleep(m_interval);
        while(m_isOpened)
        {
            if(ReadFrameForUsbCamera(m_imageData))
            {
                diplayWithImageData(m_displayLabe);
            };
        }
    }
}

void UsbCameraThread::setInterval(quint16 interval)
{
    m_interval = interval;
}

QStringList UsbCameraThread::findValidUsbCamera()
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

bool UsbCameraThread::openCameraWithIndex(quint16 index)
{
    m_pCvUsbCamera = cvCreateCameraCapture(index);
    if(m_pCvUsbCamera!=NULL)
    {
        m_isOpened = true;
        return true;
    }else
    {
        m_isOpened = false;
        return false;
    }
}

void UsbCameraThread::closeCamera()
{
    m_isOpened = false;
    this->terminate();
    if(m_isCaptureVideo) this->stopCaptureVideo();
    if(m_pCvUsbCamera!=NULL)
    {
        cvReleaseCapture(&m_pCvUsbCamera);
    }
}

bool UsbCameraThread::ReadFrameForUsbCamera(ImageData& imageBuf)
{
    if(m_isOpened){
        m_pUsbImage = cvQueryFrame(m_pCvUsbCamera);
        if(m_pUsbImage == NULL) {return false;}
        imageBuf.mutex.lock();
        imageBuf.data = QImage((const uchar*)m_pUsbImage->imageData,m_pUsbImage->width,m_pUsbImage->height,   QImage::Format_RGB888).rgbSwapped();
        imageBuf.mutex.unlock();
        if(m_isCaptureVideo)
        {
            cvWriteFrame(m_videoWriter,m_pUsbImage);
        }
        return true;
    }else{
        return false;
    }
}

void UsbCameraThread::diplayWithImageData(QLabel* dispalyLabel)
{
    if(!m_isOpened){return ;}
    if(dispalyLabel == NULL){return ;}
    //图像压缩
    IplImage *pDstImage =NULL;
    CvSize labelSize;
    labelSize.width = dispalyLabel->width();;
    labelSize.height = dispalyLabel->height();

    pDstImage = cvCreateImage(labelSize,m_pUsbImage->depth,m_pUsbImage->nChannels);
    cvResize(m_pUsbImage,pDstImage,CV_INTER_AREA);

    //显示
    QImage imgData = QImage((const uchar*)pDstImage->imageData,pDstImage->width,pDstImage->height,QImage::Format_RGB888).rgbSwapped();
    dispalyLabel->setPixmap(QPixmap::fromImage(imgData));

    cvReleaseImage(&pDstImage);
}

void UsbCameraThread::setDisplayLabel(QLabel*displayLabel)
{
    m_displayLabe = displayLabel;

}

void UsbCameraThread::capturePicture(QString fileSavePath)
{
    if(m_isOpened){
       QByteArray path = fileSavePath.toLatin1();
       cvSaveImage(path.data(),m_pUsbImage);
    }else
    {
        return ;
    }
}

void UsbCameraThread::StartCaptureVideo(QString fileSavePath)
{
    if(m_isOpened)
    {
        QByteArray path = fileSavePath.toLatin1();
        m_videoWriter = cvCreateVideoWriter(path.data(),CV_FOURCC('X', 'V', 'I', 'D'), 25, cvSize(m_pUsbImage->width,m_pUsbImage->height)); //创建CvVideoWriter对象并分配空间
        if(m_videoWriter)
        {
            qDebug()<<"video ready";
            m_isCaptureVideo = true;
        }
    }else
    {
        return;
    }
}

void UsbCameraThread::stopCaptureVideo()
{
    if(m_isOpened)
    {
        cvReleaseVideoWriter(&m_videoWriter);
        m_isCaptureVideo = false;
    }else
    {
        return;
    }
}

