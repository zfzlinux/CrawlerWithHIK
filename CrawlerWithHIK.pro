#-------------------------------------------------
#
# Project created by QtCreator 2018-10-17T15:25:50
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = CrawlerWithHIK
TEMPLATE = app
RC_FILE = logo.rc
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        $$PWD/main.cpp \
        $$PWD/mainwindow.cpp \
        $$PWD/qstartwgt.cpp \
        $$PWD/qshowmvstatus.cpp \
        $$PWD/configdlg.cpp \
        $$PWD/carmera/cameraloginsettingwgt.cpp \
            configparmeter.cpp \
    hikcamera.cpp \
    serialsettingwgt.cpp \
    crawlerstatusparam.cpp \
    crawlerserial.cpp \
    servowgt.cpp \
    sensorwgt.cpp \
    caldifspeedinsphere.cpp \
    usbcamera.cpp \
    usbcamerathread.cpp

HEADERS += \
        $$PWD/mainwindow.h \
        $$PWD/qstartwgt.h \
        $$PWD/qshowmvstatus.h \
        $$PWD/configdlg.h \
        $$PWD/carmera/cameraloginsettingwgt.h \
        configparmeter.h \
    globaldefine.h \
    hikcamera.h \
    serialsettingwgt.h \
    crawlerstatusparam.h \
    crawlerserial.h \
    servowgt.h \
    sensorwgt.h \
    caldifspeedinsphere.h \
    usbcamera.h \
    usbcamerathread.h

FORMS += \
        $$PWD/mainwindow.ui \
        $$PWD/qstartwgt.ui \
        $$PWD/qshowmvstatus.ui \
        $$PWD/configdlg.ui \
        $$PWD/carmera/cameraloginsettingwgt.ui \
    serialsettingwgt.ui \
    servowgt.ui \
    sensorwgt.ui \
    caldifspeedinsphere.ui

#Add HIK SDK_win32
INCLUDEPATH += \
    $$PWD/3rdpacket/HIK/include \
    $$PWD/3rdpacket/HIK/lib \
    $$PWD/3rdpacket/HIK/lib/HCNetSDKCom \

LIBS += \
    -L$$PWD/3rdpacket/HIK/lib/ -lHCCore \
    -L$$PWD/3rdpacket/HIK/lib/ -lHCNetSDK \
    -L$$PWD/3rdpacket/HIK/lib/ -lPlayCtrl \


#Add opencv
INCLUDEPATH += $$PWD/3rdpacked/opencv/build/include/ \
    $$PWD/3rdpacked/opencv/build/include/opencv/ \
    $$PWD/3rdpacked/opencv/build/include/opencv2/

INCLUDEPATH += E:\qtcreator_workspace\CrawlerWithHIK\3rdpacket\opencv\build\include

CONFIG(debug,debug|release){
LIBS += -L$$PWD/3rdpacket/opencv/x86/vc10/lib \
    -lopencv_core2411d \
    -lopencv_imgproc2411d \
    -lopencv_highgui2411d \
    -lopencv_ml2411d \
    -lopencv_video2411d \
    -lopencv_features2d2411d \
    -lopencv_calib3d2411d \
    -lopencv_objdetect2411d \
    -lopencv_contrib2411d \
    -lopencv_legacy2411d \
    -lopencv_flann2411d \
}
CONFIG(release,debug|release){
LIBS += -L$$PWD/3rdpacket/opencv/x86/vc10/lib \
    -lopencv_core2411 \
    -lopencv_imgproc2411 \
    -lopencv_highgui2411 \
    -lopencv_ml2411 \
    -lopencv_video2411 \
    -lopencv_features2d2411 \
    -lopencv_calib3d2411 \
    -lopencv_objdetect2411 \
    -lopencv_contrib2411 \
    -lopencv_legacy2411 \
    -lopencv_flann2411 \
}

RESOURCES += \
    resource.qrc

DISTFILES += \
    logo.rc
