#-------------------------------------------------
#
# Project created by QtCreator 2013-09-07T03:40:58
#
#-------------------------------------------------

QT       += network
QT       -= gui

TARGET = qkapi
TEMPLATE = lib

DEFINES += QT_NO_DEBUG_OUTPUT

DEFINES += QKAPI_LIBRARY

SOURCES += qkapi.cpp \
    qkapisocket.cpp \
    jsonrpcfactory.cpp

HEADERS += qkapi.h\
        qkapi_global.h \
    qkapisocket.h \
    jsonrpcfactory.h

RESOURCES += \
    resources/qkapi.qrc

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

CONFIG(debug, debug|release) {
    DESTDIR = debug
} else {
    DESTDIR = release
}

OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui


