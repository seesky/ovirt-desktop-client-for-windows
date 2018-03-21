#-------------------------------------------------
#
# Project created by QtCreator 2017-06-19T17:44:39
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    settingdialog.cpp \
    statusupdatethread.cpp \
    utils.cpp \
    pingthread.cpp \
    execcmd.cpp \
    mwidget.cpp \
    httprequest.cpp \
    qprogressindicator.cpp \
    sessionstatusupdatethread.cpp \
    vmlistwindow.cpp \
    vmele.cpp \
    vmstatusupdatethread.cpp \
    flowlayout.cpp

HEADERS += \
        mainwindow.h \
    settingdialog.h \
    statusupdatethread.h \
    utils.h \
    IPEXPORT.H \
    Iphlpapi.h \
    IPRTRMIB.H \
    IPTYPES.H \
    pingthread.h \
    execcmd.h \
    mwidget.h \
    httprequest.h \
    qprogressindicator.h \
    sessionstatusupdatethread.h \
    vmlistwindow.h \
    vmele.h \
    vmstatusupdatethread.h \
    flowlayout.h

FORMS += \
        mainwindow.ui \
    settingdialog.ui \
    vmlistwindow.ui \
    vmele.ui

DISTFILES +=

RESOURCES += \
    thinclientsource.qrc

LIBS += -LF:D:\Qt\QtProject\untitled\lib -lIPHLPAPI
