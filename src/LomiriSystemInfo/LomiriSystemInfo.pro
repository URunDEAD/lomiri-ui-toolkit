TARGET = LomiriSystemInfo
QT = core network dbus

HEADERS += $$PWD/linux/lomiribatteryinfo_upower_p.h \
           $$PWD/linux/lomirideviceinfo_linux_p.h \
           $$PWD/linux/lomiridevicekitservice_linux_p.h \
           $$PWD/linux/lomiriinputinfomanagerudev_p.h \
           $$PWD/linux/lomirinetworkinfo_linux_p.h \
           $$PWD/linux/lomiriofonowrapper_p.h \
           $$PWD/linux/lomiriscreensaver_mir_p.h \
           $$PWD/linux/lomiriudevwrapper_p.h \
           $$PWD/lomiribatteryinfo.h \
           $$PWD/lomirideviceinfo.h \
           $$PWD/lomiriinputinfo.h \
           $$PWD/lomiriinputinfomanager.h \
           $$PWD/lomiriinputinfomanager_p.h \
           $$PWD/lomirinetworkinfo.h \
           $$PWD/lomiriscreensaver.h

SOURCES += $$PWD/linux/lomiribatteryinfo_upower.cpp \
           $$PWD/linux/lomirideviceinfo_linux.cpp \
           $$PWD/linux/lomiridevicekitservice_linux.cpp \
           $$PWD/linux/lomiriinputinfomanagerudev.cpp \
           $$PWD/linux/lomirinetworkinfo_linux.cpp \
           $$PWD/linux/lomiriofonowrapper.cpp \
           $$PWD/linux/lomiriscreensaver_mir.cpp \
           $$PWD/linux/lomiriudevwrapper.cpp \
           $$PWD/lomiribatteryinfo.cpp \
           $$PWD/lomirideviceinfo.cpp \
           $$PWD/lomiriinputinfo.cpp \
           $$PWD/lomiriinputinfomanager.cpp \
           $$PWD/lomirinetworkinfo.cpp \
           $$PWD/lomiriscreensaver.cpp

DEFINES += QT_UNITY8

CONFIG += link_pkgconfig
PKGCONFIG += x11 udev libevdev bluez
LIBS += -ludev -levdev

config_mir {
    QT += gui gui-private
    CONFIG += link_pkgconfig
    PKGCONFIG += mirclient
    LIBS += -lmirclient
    SOURCES += $$PWD/linux/lomiriinputinfomanagermir.cpp
    HEADERS += $$PWD/linux/lomiriinputinfomanagermir_p.h
} else {
    DEFINES += QT_NO_MIR
}

load(lomiri_qt_module)