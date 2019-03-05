#include "hikcamera.h"

HIKCamera::HIKCamera(QObject *parent) : QObject(parent),
    m_displayHWND(NULL),
    m_isOpened(false)
{

}

HIKCamera::~HIKCamera()
{
    if(m_isOpened)
    {
        this->closeHIKCamera();
    }
}

void HIKCamera::setCameraLoginInfo(StruCameraCntCfg cntCfg)
{
    m_cameraCntcfg = cntCfg;
}

void HIKCamera::setDisplayWidgetHWND(HWND hwnd)
{
    this->m_displayHWND = hwnd;
}

bool HIKCamera::openHIKCamera()
{
    if(m_displayHWND ==NULL) return false;

    if(!initHIKSDK()) return false;

    m_lUserID = loginDevice();

    if(m_lUserID<0)
    {
        NET_DVR_Cleanup();
        return false;
    }
    //
    if(m_lUserID == NET_DVR_NOERROR)
    {
        //show
        m_isOpened = realplay();
        return m_isOpened;
    }
    m_isOpened = true;
    return m_isOpened;
}

void HIKCamera::closeHIKCamera()
{
    //关闭预览
     NET_DVR_StopRealPlay(m_lRealPlayHandle);
     //注销用户
     NET_DVR_Logout(m_lUserID);
     //释放SDK资源
     NET_DVR_Cleanup();
     m_isOpened =false;
}

bool HIKCamera::initHIKSDK()
{
    //must config before initsdk
    DWORD dwErrCode =0;
    NET_DVR_INIT_CHECK_MODULE_COM struInitCheckModuleCom;
    memset(&struInitCheckModuleCom,0,sizeof(NET_DVR_INIT_CHECK_MODULE_COM));
    struInitCheckModuleCom.byEnable = 1;//默认启用
    if(!NET_DVR_SetSDKInitCfg(NET_SDK_INIT_CFG_TYPE_CHECK_MODULE_COM,&struInitCheckModuleCom))
    {
        dwErrCode = NET_DVR_GetLastError();
    }
    //布防默认最大为2048路
    NET_DVR_INIT_CFG_ABILITY struCfgAbility = { INIT_CFG_NUM_2048, INIT_CFG_NUM_2048, 0 };
    if(!NET_DVR_SetSDKInitCfg(NET_SDK_INIT_CFG_ABILITY,&struCfgAbility))
    {
        dwErrCode = NET_DVR_GetLastError();
    }
    // init sdk
    if(!NET_DVR_Init())
    {
        dwErrCode = NET_DVR_GetLastError();
        return false;
    }
    return true;
}

long HIKCamera::loginDevice()
{
    long lUserID;
    std::string sIPAddress,sUserName,sPassword;
    WORD wPort;
    sIPAddress  = m_cameraCntcfg.IPAddress.toStdString();
    sUserName = m_cameraCntcfg.UserName.toStdString();
    sPassword =m_cameraCntcfg.Password.toStdString();
    wPort = m_cameraCntcfg.port;

    //config
    NET_DVR_USER_LOGIN_INFO struUserInfo;
    memset(&struUserInfo,0,sizeof(NET_DVR_USER_LOGIN_INFO));
    struUserInfo.bUseAsynLogin = 0; // 同步登陆方式
    strcpy(struUserInfo.sDeviceAddress,sIPAddress.data());
    strcpy(struUserInfo.sUserName,sUserName.data());
    strcpy(struUserInfo.sPassword,sPassword.data());
    struUserInfo.wPort = wPort;

    //Device infomation  output for parmeter
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
    lUserID = NET_DVR_Login_V40(&struUserInfo,&struDeviceInfoV40);
    return lUserID;
}

bool HIKCamera::realplay()
{
    NET_DVR_PREVIEWINFO struPlayInfo = {0};
    struPlayInfo.hPlayWnd = m_displayHWND;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
    struPlayInfo.lChannel     = 1;       //预览通道号
    struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
    struPlayInfo.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
    struPlayInfo.bBlocked     = 1;       //0- 非阻塞取流，1- 阻塞取流

    m_lRealPlayHandle = NET_DVR_RealPlay_V40(m_lUserID, &struPlayInfo, NULL, NULL);
    if(m_lRealPlayHandle < 0)
    {
        NET_DVR_Logout(m_lUserID);
        NET_DVR_Cleanup();
        return false;
    }
    return true;
}

DWORD HIKCamera::getErrCode()
{
    return NET_DVR_GetLastError();
}

void HIKCamera::capturePicture(QString fileSavePath)
{
    if(m_isOpened)
    {
        //设置模式
        if(!NET_DVR_SetCapturePictureMode(BMP_MODE)) return ;
        //
        if(NET_DVR_CapturePicture(m_lRealPlayHandle,fileSavePath.toLatin1().data()))
        {
        }
    }
}

bool HIKCamera::StartCaptureVideo(QString fileSavePath)
{
    bool isStart =false;
    isStart= NET_DVR_SaveRealData(m_lRealPlayHandle,fileSavePath.toLatin1().data());
    return isStart;
}

bool HIKCamera::StopCaptureVideo()
{
    bool isStop =false;
    isStop= NET_DVR_StopRealPlay(m_lRealPlayHandle);
    return isStop;
}
