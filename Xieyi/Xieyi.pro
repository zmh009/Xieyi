#-------------------------------------------------
#
# Project created by QtCreator 2017-02-17T07:50:55
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Xieyi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ZSetupWidget.cpp \
    ZResponseWidget.cpp \
    ZControlTask.cpp \
    ZTask.cpp \
    ZCapture.cpp \
    ZBuffer.cpp \
    ZRestore.cpp \
    ZFile.cpp \
    ZSQL.cpp \
    ZHttp.cpp \
    ZHttps.cpp \
    ZIpv4.cpp \
    ZTcp.cpp \
    ZUdp.cpp \
    ZMac.cpp \
    ZInformation.cpp \
    ZBehaviorAnalysis.cpp \
    ZBeheviorResponse.cpp

HEADERS  += mainwindow.h \
    ZSetupWidget.h \
    ZResponseWidget.h \
    ZControlTask.h \
    ZTask.h \
    ZCapture.h \
    ZBuffer.h \
    ZRestore.h \
    ZFile.h \
    ZSQL.h \
    ZProcotol.h \
    ZHttp.h \
    ZHttps.h \
    ZIpv4.h \
    ZTcp.h \
    ZUdp.h \
    ITransport.h \
    IApplication.h \
    ZMac.h \
    INetwork.h \
    ZInformation.h \
    ZBehaviorAnalysis.h \
    ZBeheviorResponse.h

FORMS    += mainwindow.ui

CONFIG += \
        c++11

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lpcap
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lpcap
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lpcap

LIBS += -lconfig++

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

RESOURCES += \
    ZResource.qrc

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -lmysqlclient

INCLUDEPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
