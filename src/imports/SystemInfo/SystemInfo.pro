CXX_MODULE = qml
TARGET  = LomiriSystemInfo
TARGETPATH = Lomiri/SystemInfo
IMPORT_VERSION = 0.1

INCLUDEPATH+=$$PWD

QT += qml LomiriSystemInfo
QT -= gui

HEADERS += \
    lomirideviceinfo_p.h \
    lomirinetworkinfo_p.h

SOURCES += \
    lomirideviceinfo.cpp \
    lomirinetworkinfo.cpp \
    lomirisysteminfo.cpp

linux-*: {
    HEADERS += \
        lomiriinputdevicemodel_p.h
    SOURCES += \
        lomiriinputdevicemodel.cpp \
}

load(lomiri_qml_plugin)
