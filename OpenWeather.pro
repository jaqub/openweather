#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T16:03:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenWeather
TEMPLATE = app
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
        main.cpp \
    openweather.cpp \
    weatheritemdelegate.cpp \
    weatherlistitem.cpp \
    udev.cpp \
    logger.cpp \
    device.cpp \
    backlightdev.cpp

HEADERS += \
    openweather.h \
    weatheritemdelegate.h \
    weatherlistitem.h \
    udev.h \
    singleton.h \
    logger.h \
    device.h \
    backlightdev.h

FORMS += \
    openweathermain.ui

LIBS += -ludev

target.path = /home/pi
INSTALLS += target

RESOURCES += \
    openweathermain.qrc
