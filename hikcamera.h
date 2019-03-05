#ifndef HIKCAMERA_H
#define HIKCAMERA_H

#include <QObject>
#include <globaldefine.h>
#include <HCNetSDK.h>

class HIKCamera : public QObject
{
    Q_OBJECT
public:
    explicit HIKCamera(QObject *parent = nullptr);
    ~HIKCamera();
public:
    void setCameraLoginInfo(StruCameraCntCfg cntCfg);
    void setDisplayWidgetHWND(HWND hwnd);
    bool openHIKCamera();
    void closeHIKCamera();
    DWORD getErrCode();
    void capturePicture(QString fileSavePath);
    bool StartCaptureVideo(QString fileSavePath);
    bool StopCaptureVideo();
private:
    bool initHIKSDK();
    long loginDevice();
    bool realplay();
private:
    bool m_isOpened;
    StruCameraCntCfg m_cameraCntcfg;
    HWND m_displayHWND;
    long m_lUserID,m_lRealPlayHandle;
};

#endif // HIKCAMERA_H
