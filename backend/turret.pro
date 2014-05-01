#-------------------------------------------------
#
# Project created by QtCreator 2014-04-27T05:27:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = turret
TEMPLATE = app

INCLUDEPATH += D:/OpenCV/opencv/build/include
LIBS += -LD:/OpenCV/opencv/build/x86/vc10/lib \
    -lopencv_core248d \
    -lopencv_highgui248d \
    -lopencv_imgproc248d

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
