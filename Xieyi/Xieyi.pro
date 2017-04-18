#-------------------------------------------------
#
# Project created by QtCreator 2017-02-17T07:50:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Xieyi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ZSetupWidget.cpp \
    ZResponseWidget.cpp \
    ZControlTask.cpp \
    ZTask.cpp \
    ZPcapOption.cpp \
    ZCapture.cpp \
    ZBuffer.cpp \
    ZNetworkData.cpp \
    ZRestore.cpp \
    ZFile.cpp \
    ZSQL.cpp \
    ZResponse.cpp \
    ZHttp.cpp \
    ZHttps.cpp \
    ZIpv4.cpp \
    ZTcp.cpp \
    ZUdp.cpp \
    ZMac.cpp \
    ZInformation.cpp

HEADERS  += mainwindow.h \
    ZSetupWidget.h \
    ZResponseWidget.h \
    ZControlTask.h \
    ZTask.h \
    ZPcapOption.h \
    ZCapture.h \
    ZBuffer.h \
    ZNetworkData.h \
    ZRestore.h \
    ZFile.h \
    ZSQL.h \
    ZProcotol.h \
    ZResponse.h \
    ZHttp.h \
    ZHttps.h \
    ZIpv4.h \
    ZTcp.h \
    ZUdp.h \
    ITransport.h \
    IApplication.h \
    ZMac.h \
    INetwork.h \
    ZInformation.h

FORMS    += mainwindow.ui

CONFIG += \
        c++11

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lpcap
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lpcap
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lpcap

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

RESOURCES += \
    ZResource.qrc
