#-------------------------------------------------
#
# Project created by QtCreator 2013-03-20T14:30:19
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = qmqtt
TEMPLATE = lib

DEFINES += QMQTT_LIBRARY

CONFIG(debug, debug|release) {
    OBJDIR = bin/debug
} else {
    OBJDIR = bin/release
}

linux-g++{
    64B{
        message( "qmqtt: Configuring for amd64")
        SUFFIX = amd64
        DEFINES += USE64B
        DESTDIR = ../../libs/amd64
    } else {
        message( "qmqtt: Configuring for i386")
        SUFFIX = i386
        DESTDIR = ../../libs/i386
    }
}
linux-g++-64{
    message( "qmqtt: Configuring for amd64")
    SUFFIX = amd64
    DEFINES += USE64B
    DESTDIR = ../../libs/amd64
}
linux-g++-32{
    message( "qmqtt: Configuring for i386")
    DESTDIR  = ../../libs/i386
    SUFFIX = i386
}
linux-avr32-g++{
    message( "qmqtt: Configuring for Avr32")
    DESTDIR = ../../libs/avr32
    QMAKE_CFLAGS_RELEASE -= -O2
    QMAKE_CFLAGS_RELEASE *= -O
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE *= -O
    SUFFIX = avr
}
linux-arm-gnueabi-g++|linux-armel-gnueabi-g++{
    message( "qmqtt: Configuring for ARMv5")
    DEFINES += P2NRGENABLE
    DESTDIR = ../../libs/arm
    SUFFIX = arm
}
linux-armv6-gnueabihf-g++|linux-rpi-gnueabihf-g++{
    message( "qmqtt: Configuring for ARMv6 vfp")
    DESTDIR = ../../libs/armv6
    SUFFIX = armv6
}
linux-armhf-gnueabi-g++|linux-armG1-gnueabihf-g++{
    message( "qmqtt: Configuring for ARMv7")
    DESTDIR = ../../libs/armv7
    SUFFIX = armv7
}
linux-armhfandroid-gnueabi-g++{
    message( "qmqtt: Configuring for ARMv7 Android")
    DESTDIR = ../../libs/armv7_android
    SUFFIX = armv7_android
}
linux-arm-uclibcgnueabi-g++{
    message( "qmqtt: Configuring for BCM-97445C")
    DESTDIR = ../../libs/bcm
    SUFFIX = bcm
}
linux-arm-gnueabihf-g++|linux-armbcm-gnueabihf-g++{
    message("qmqtt: Configuring for BCM-ARMHF")
    DESTDIR  = ../../libs/bcmhf
    SUFFIX = bcmhf
}

OBJECTS_DIR = $${OBJDIR}/.obj$${SUFFIX}
MOC_DIR = $${OBJDIR}/.moc$${SUFFIX}
RCC_DIR = $${OBJDIR}/.rcc$${SUFFIX}
UI_DIR = $${OBJDIR}/.ui$${SUFFIX}

DEPENDPATH += . ../QtWebsocket/QtWebsocket
INCLUDEPATH += ../QtWebsocket/QtWebsocket

LIBS += -L ../QtWebsocket/QtWebsocket -lQtWebsocket

SOURCES += qmqtt_client.cpp \
    qmqtt_network.cpp \
    qmqtt_frame.cpp \
    qmqtt_client_p.cpp \
    qmqtt_message.cpp \
    qmqtt_router.cpp \
    qmqtt_will.cpp \
    qmqtt_routesubscription.cpp \
    qmqtt_routedmessage.cpp \
    qmqtt_wss_network.cpp

HEADERS += qmqtt_client.h\
    qmqtt_global.h \
    qmqtt_network.h \
    qmqtt_frame.h \
    qmqtt_client_p.h \
    qmqtt_message.h \
    qmqtt_will.h \
    qmqtt_router.h \
    qmqtt.h \
    qmqtt_routesubscription.h \
    qmqtt_routedmessage.h \
    qmqtt_wss_network.h \
    qmqtt_abstract_network.h

isEmpty(PREFIX) {
    contains(MEEGO_EDITION,harmattan) {
        PREFIX = /usr
    } else:unix:!symbian {
        maemo5 {
            PREFIX = /opt/usr
        } else {
            PREFIX = /usr/local
        }
    } else {
        PREFIX = $$[QT_INSTALL_PREFIX]
    }
}

headers.files = $$HEADERS
headers.path = $$PREFIX/include/qmqtt
target.path = $$PREFIX/lib
INSTALLS += headers target

OTHER_FILES += \
    qmqtt.pri
