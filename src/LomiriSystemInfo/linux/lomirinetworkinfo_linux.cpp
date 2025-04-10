/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSystems module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "lomirinetworkinfo_linux_p.h"

#if !defined(QT_NO_OFONO)
#include "lomiriofonowrapper_p.h"
#endif

#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qtimer.h>
#if !defined(QT_NO_BLUEZ)
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#endif // QT_NO_BLUEZ

#if !defined(QT_NO_UDEV)
#include <QtCore/qsocketnotifier.h>
#include <libudev.h>
#endif // QT_NO_UDEV

#include <math.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/wireless.h>
#include <unistd.h>

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC_WITH_ARGS(const QString, NETWORK_SYSFS_PATH, (QLatin1String("/sys/class/net/")))

Q_GLOBAL_STATIC_WITH_ARGS(const QStringList, WLAN_MASK, (QStringList() << QLatin1String("wlan*") << QLatin1String("wlp*")))
Q_GLOBAL_STATIC_WITH_ARGS(const QStringList, ETHERNET_MASK, (QStringList() << QLatin1String("eth*") << QLatin1String("usb*") << QLatin1String("enp*")))

LomiriNetworkInfoPrivate::LomiriNetworkInfoPrivate(LomiriNetworkInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , watchCurrentNetworkMode(false)
    , watchNetworkInterfaceCount(false)
    , watchNetworkSignalStrength(false)
    , watchNetworkStatus(false)
    , watchNetworkName(false)
    , timer(0)
#if !defined(QT_NO_OFONO)
    , ofonoWrapper(0)
#endif
#if !defined(QT_NO_UDEV)
    , udevNotifier(0)
    , udevHandle(0)
    , udevMonitor(0)
#endif // QT_NO_UDEV
{
}

LomiriNetworkInfoPrivate::~LomiriNetworkInfoPrivate()
{
#if !defined(QT_NO_UDEV)
    if (udevMonitor)
        udev_monitor_unref(udevMonitor);

    if (udevHandle)
        udev_unref(udevHandle);
#endif // QT_NO_UDEV
}

int LomiriNetworkInfoPrivate::networkInterfaceCount(LomiriNetworkInfo::NetworkMode mode)
{
    if (watchNetworkInterfaceCount && (mode == LomiriNetworkInfo::WlanMode
                                       || mode == LomiriNetworkInfo::EthernetMode
                                       || mode == LomiriNetworkInfo::BluetoothMode)) {
        return networkInterfaceCounts.value(mode);
    } else
        return getNetworkInterfaceCount(mode);
}

int LomiriNetworkInfoPrivate::networkSignalStrength(LomiriNetworkInfo::NetworkMode mode, int interface)
{
    if (watchNetworkSignalStrength && (mode == LomiriNetworkInfo::WlanMode
                                       || mode == LomiriNetworkInfo::EthernetMode
                                       || mode == LomiriNetworkInfo::BluetoothMode)) {
        return networkSignalStrengths.value(QPair<LomiriNetworkInfo::NetworkMode, int>(mode, interface));
    } else
        return getNetworkSignalStrength(mode, interface);
}

LomiriNetworkInfo::CellDataTechnology LomiriNetworkInfoPrivate::currentCellDataTechnology(int interface)
{
#if !defined(QT_NO_OFONO)
    if (QOfonoWrapper::isOfonoAvailable()) {
        if (!ofonoWrapper)
            ofonoWrapper = new QOfonoWrapper(this);
        QStringList modems = ofonoWrapper->allModems();
        if (interface < modems.size()) {
            QString modem = ofonoWrapper->allModems().at(interface);
            if (!modem.isEmpty())
                return ofonoWrapper->currentCellDataTechnology(modem);
        }
    }
#else
    Q_UNUSED(interface)
#endif
    return LomiriNetworkInfo::UnknownDataTechnology;
}

LomiriNetworkInfo::NetworkMode LomiriNetworkInfoPrivate::currentNetworkMode()
{
    if (watchCurrentNetworkMode)
        return currentMode;
    else
        return getCurrentNetworkMode();
}

LomiriNetworkInfo::NetworkStatus LomiriNetworkInfoPrivate::networkStatus(LomiriNetworkInfo::NetworkMode mode, int interface)
{
    if (watchNetworkStatus && (mode == LomiriNetworkInfo::WlanMode
                               || mode == LomiriNetworkInfo::EthernetMode
                               || mode == LomiriNetworkInfo::BluetoothMode)) {
        return networkStatuses.value(QPair<LomiriNetworkInfo::NetworkMode, int>(mode, interface));
    } else
        return getNetworkStatus(mode, interface);
}

#ifndef QT_NO_NETWORKINTERFACE
QNetworkInterface LomiriNetworkInfoPrivate::interfaceForMode(LomiriNetworkInfo::NetworkMode mode, int interface)
{
    switch (mode) {
    case LomiriNetworkInfo::WlanMode: {
        QStringList dirs = QDir(*NETWORK_SYSFS_PATH()).entryList(*WLAN_MASK());
        if (interface < dirs.size()) {
            QNetworkInterface networkInterface = QNetworkInterface::interfaceFromName(dirs.at(interface));
            if (networkInterface.isValid())
                return networkInterface;
        }
        break;
    }

    case LomiriNetworkInfo::EthernetMode: {
        QStringList dirs = QDir(*NETWORK_SYSFS_PATH()).entryList(*ETHERNET_MASK());
        if (interface < dirs.size()) {
            QNetworkInterface networkInterface = QNetworkInterface::interfaceFromName(dirs.at(interface));
            if (networkInterface.isValid())
                return networkInterface;
        }
        break;
    }

//    case LomiriNetworkInfo::BluetoothMode:
//    case LomiriNetworkInfo::GsmMode:
//    case LomiriNetworkInfo::CdmaMode:
//    case LomiriNetworkInfo::WcdmaMode:
//    case LomiriNetworkInfo::WimaxMode:
//    case LomiriNetworkInfo::LteMode:
//    case LomiriNetworkInfo::TdscdmaMode:
    default:
        break;
    };

    return QNetworkInterface();
}
#endif // QT_NO_NETWORKINTERFACE

QString LomiriNetworkInfoPrivate::cellId(int interface)
{
#if !defined(QT_NO_OFONO)
    if (QOfonoWrapper::isOfonoAvailable()) {
        if (!ofonoWrapper)
            ofonoWrapper = new QOfonoWrapper(this);
        QStringList modems = ofonoWrapper->allModems();
        if (interface < modems.size()) {
            QString modem = ofonoWrapper->allModems().at(interface);
            if (!modem.isEmpty())
                return ofonoWrapper->cellId(modem);
        }
    }
#else
    Q_UNUSED(interface)
#endif
    return QString();
}

QString LomiriNetworkInfoPrivate::currentMobileCountryCode(int interface)
{
#if !defined(QT_NO_OFONO)
    if (QOfonoWrapper::isOfonoAvailable()) {
        if (!ofonoWrapper)
            ofonoWrapper = new QOfonoWrapper(this);
        QStringList modems = ofonoWrapper->allModems();
        if (interface < modems.size()) {
            QString modem = ofonoWrapper->allModems().at(interface);
            if (!modem.isEmpty())
                return ofonoWrapper->currentMcc(modem);
        }
    }
#else
    Q_UNUSED(interface)
#endif
    return QString();
}

QString LomiriNetworkInfoPrivate::currentMobileNetworkCode(int interface)
{
#if !defined(QT_NO_OFONO)
    if (QOfonoWrapper::isOfonoAvailable()) {
        if (!ofonoWrapper)
            ofonoWrapper = new QOfonoWrapper(this);
        QStringList modems = ofonoWrapper->allModems();
        if (interface < modems.size()) {
            QString modem = ofonoWrapper->allModems().at(interface);
            if (!modem.isEmpty())
                return ofonoWrapper->currentMnc(modem);
        }
    }
#else
    Q_UNUSED(interface)
#endif
    return QString();
}

QString LomiriNetworkInfoPrivate::homeMobileCountryCode(int interface)
{
#if !defined(QT_NO_OFONO)
    if (QOfonoWrapper::isOfonoAvailable()) {
        if (!ofonoWrapper)
            ofonoWrapper = new QOfonoWrapper(this);
        QStringList modems = ofonoWrapper->allModems();
        if (interface < modems.size()) {
            QString modem = ofonoWrapper->allModems().at(interface);
            if (!modem.isEmpty())
                return ofonoWrapper->homeMcc(modem);
        }
    }
#else
    Q_UNUSED(interface)
#endif
    return QString();
}

QString LomiriNetworkInfoPrivate::homeMobileNetworkCode(int interface)
{
#if !defined(QT_NO_OFONO)
    if (QOfonoWrapper::isOfonoAvailable()) {
        if (!ofonoWrapper)
            ofonoWrapper = new QOfonoWrapper(this);
        QStringList modems = ofonoWrapper->allModems();
        if (interface < modems.size()) {
            QString modem = ofonoWrapper->allModems().at(interface);
            if (!modem.isEmpty())
                return ofonoWrapper->homeMnc(modem);
        }
    }
#else
    Q_UNUSED(interface)
#endif
    return QString();
}

QString LomiriNetworkInfoPrivate::imsi(int interface)
{
#if !defined(QT_NO_OFONO)
    if (QOfonoWrapper::isOfonoAvailable()) {
        if (!ofonoWrapper)
            ofonoWrapper = new QOfonoWrapper(this);
        QStringList modems = ofonoWrapper->allModems();
        if (interface < modems.size()) {
            QString modem = ofonoWrapper->allModems().at(interface);
            if (!modem.isEmpty())
                return ofonoWrapper->imsi(modem);
        }
    }
#else
    Q_UNUSED(interface)
#endif
    return QString();
}

QString LomiriNetworkInfoPrivate::locationAreaCode(int interface)
{
#if !defined(QT_NO_OFONO)
    if (QOfonoWrapper::isOfonoAvailable()) {
        if (!ofonoWrapper)
            ofonoWrapper = new QOfonoWrapper(this);
        QStringList modems = ofonoWrapper->allModems();
        if (interface < modems.size()) {
            QString modem = ofonoWrapper->allModems().at(interface);
            if (!modem.isEmpty())
                return ofonoWrapper->lac(modem);
        }
    }
#else
    Q_UNUSED(interface)
#endif
    return QString();
}

QString LomiriNetworkInfoPrivate::macAddress(LomiriNetworkInfo::NetworkMode mode, int interface)
{
    switch (mode) {
    case LomiriNetworkInfo::WlanMode: {
        QStringList dirs = QDir(*NETWORK_SYSFS_PATH()).entryList(*WLAN_MASK());
        if (interface < dirs.size()) {
            QFile carrier(*NETWORK_SYSFS_PATH() + dirs.at(interface) + QString(QStringLiteral("/address")));
            if (carrier.open(QIODevice::ReadOnly))
                return QString::fromLatin1(carrier.readAll().simplified().data());
        }
        break;
    }

    case LomiriNetworkInfo::EthernetMode: {
        QStringList dirs = QDir(*NETWORK_SYSFS_PATH()).entryList(*ETHERNET_MASK());
        if (interface < dirs.size()) {
            QFile carrier(*NETWORK_SYSFS_PATH() + dirs.at(interface) + QString(QStringLiteral("/address")));
            if (carrier.open(QIODevice::ReadOnly))
                return QString::fromLatin1(carrier.readAll().simplified().data());
        }
        break;
    }

    case LomiriNetworkInfo::BluetoothMode: {
#if !defined(QT_NO_BLUEZ)
        int ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
        if (ctl < 0)
            break;
        struct hci_dev_list_req *deviceList = (struct hci_dev_list_req *)malloc(HCI_MAX_DEV * sizeof(struct hci_dev_req) + sizeof(uint16_t));
        deviceList->dev_num = HCI_MAX_DEV;
        QString macAddress;
        if (ioctl(ctl, HCIGETDEVLIST, deviceList) == 0) {
            int count = deviceList->dev_num;
            if (interface < count) {
                struct hci_dev_info deviceInfo;
                deviceInfo.dev_id = (deviceList->dev_req + interface)->dev_id;
                if (ioctl(ctl, HCIGETDEVINFO, &deviceInfo) == 0) {
                    // do not use BDADDR_ANY, fails with gcc 4.6
                    bdaddr_t bdaddr_any = (bdaddr_t) {{0, 0, 0, 0, 0, 0}};
                    if (hci_test_bit(HCI_RAW, &deviceInfo.flags) && !bacmp(&deviceInfo.bdaddr, &bdaddr_any)) {
                        int hciDevice = hci_open_dev(deviceInfo.dev_id);
                        hci_read_bd_addr(hciDevice, &deviceInfo.bdaddr, 1000);
                        hci_close_dev(hciDevice);
                    }
                    char address[18];
                    ba2str(&deviceInfo.bdaddr, address);
                    macAddress = QString::fromLatin1(address);
                }
            }
        }
        free(deviceList);
        close(ctl);
        return macAddress;
#else
        break;
#endif // QT_NO_BLUEZ
    }

//    case LomiriNetworkInfo::GsmMode:
//    case LomiriNetworkInfo::CdmaMode:
//    case LomiriNetworkInfo::WcdmaMode:
//    case LomiriNetworkInfo::WimaxMode:
//    case LomiriNetworkInfo::LteMode:
//    case LomiriNetworkInfo::TdscdmaMode:
    default:
        break;
    };

    return QString();
}

QString LomiriNetworkInfoPrivate::networkName(LomiriNetworkInfo::NetworkMode mode, int interface)
{
    if (watchNetworkName && (mode == LomiriNetworkInfo::WlanMode
                             || mode == LomiriNetworkInfo::EthernetMode
                             || mode == LomiriNetworkInfo::BluetoothMode)) {
        return networkNames.value(QPair<LomiriNetworkInfo::NetworkMode, int>(mode, interface));
    } else
        return getNetworkName(mode, interface);
}

extern QMetaMethod proxyToSourceSignal(const QMetaMethod &, QObject *);

void LomiriNetworkInfoPrivate::connectNotify(const QMetaMethod &signal)
{
#if !defined(QT_NO_OFONO)
    if (QOfonoWrapper::isOfonoAvailable()) {
        if (!ofonoWrapper)
            ofonoWrapper = new QOfonoWrapper(this);
        QMetaMethod sourceSignal = proxyToSourceSignal(signal, ofonoWrapper);
        connect(ofonoWrapper, sourceSignal, this, signal, Qt::UniqueConnection);
    }
#endif

    static const QMetaMethod currentNetworkModeChangedSignal = QMetaMethod::fromSignal(&LomiriNetworkInfoPrivate::currentNetworkModeChanged);
    static const QMetaMethod networkNameChangedSignal = QMetaMethod::fromSignal(&LomiriNetworkInfoPrivate::networkNameChanged);
    static const QMetaMethod networkSignalStrengthChangedSignal = QMetaMethod::fromSignal(&LomiriNetworkInfoPrivate::networkSignalStrengthChanged);
    static const QMetaMethod networkStatusChangedSignal = QMetaMethod::fromSignal(&LomiriNetworkInfoPrivate::networkStatusChanged);

    //    we always monitor "networkInterfaceCount" , as long as users connect any signals,
    //    with update to date network interface counts, we can compute network mode, strength,
    //    status, name properties in an efficient way
    if (!watchNetworkInterfaceCount) {
        QList<LomiriNetworkInfo::NetworkMode> modes;
        modes << LomiriNetworkInfo::WlanMode << LomiriNetworkInfo::EthernetMode << LomiriNetworkInfo::BluetoothMode;
        networkInterfaceCounts.clear();
        Q_FOREACH (LomiriNetworkInfo::NetworkMode mode, modes)
            networkInterfaceCounts[mode] = getNetworkInterfaceCount(mode);
#if !defined(QT_NO_UDEV)
        if (!udevHandle) {
            udevHandle = udev_new();
            udevMonitor = udev_monitor_new_from_netlink(udevHandle, "udev");
            if (udevMonitor) {
                udev_monitor_filter_add_match_subsystem_devtype(udevMonitor, "net", NULL);
                udev_monitor_enable_receiving(udevMonitor);
                udevNotifier = new QSocketNotifier(udev_monitor_get_fd(udevMonitor), QSocketNotifier::Read, this);
                connect(udevNotifier, SIGNAL(activated(int)), this, SLOT(onUdevChanged()));
            }
        }
        if (udevNotifier) {
            udevNotifier->setEnabled(true);
        }

#endif // QT_NO_UDEV
        watchNetworkInterfaceCount = true;
    }

    if (signal == currentNetworkModeChangedSignal) {
//        we monitor "networkStatus" by default, as long as currentNetworkModeChanged signal
//        is connected, with always up to date network status, current network mode can
//        be fast computed.
        if (!watchNetworkStatus) {
            QList<LomiriNetworkInfo::NetworkMode> modes;
            modes << LomiriNetworkInfo::WlanMode << LomiriNetworkInfo::EthernetMode << LomiriNetworkInfo::BluetoothMode;
            networkStatuses.clear();
            Q_FOREACH (LomiriNetworkInfo::NetworkMode mode, modes) {
                int count = networkInterfaceCount(mode);
                for (int i = 0; i < count; ++i)
                    networkStatuses[QPair<LomiriNetworkInfo::NetworkMode, int>(mode, i)] = getNetworkStatus(mode, i);
            }
            watchNetworkStatus = true;
        }
        currentMode = getCurrentNetworkMode();
        watchCurrentNetworkMode = true;
    } else if (signal == networkSignalStrengthChangedSignal) {
        QList<LomiriNetworkInfo::NetworkMode> modes;
        modes << LomiriNetworkInfo::WlanMode << LomiriNetworkInfo::EthernetMode << LomiriNetworkInfo::BluetoothMode;
        networkSignalStrengths.clear();
        Q_FOREACH (LomiriNetworkInfo::NetworkMode mode, modes) {
            int count = networkInterfaceCount(mode);
            for (int i = 0; i < count; ++i)
                networkSignalStrengths[QPair<LomiriNetworkInfo::NetworkMode, int>(mode, i)] = getNetworkSignalStrength(mode, i);
        }

        watchNetworkSignalStrength = true;
    } else if (signal == networkStatusChangedSignal) {
        QList<LomiriNetworkInfo::NetworkMode> modes;
        modes << LomiriNetworkInfo::WlanMode << LomiriNetworkInfo::EthernetMode << LomiriNetworkInfo::BluetoothMode;
        networkStatuses.clear();
        Q_FOREACH (LomiriNetworkInfo::NetworkMode mode, modes) {
            int count = networkInterfaceCount(mode);
            for (int i = 0; i < count; ++i)
                networkStatuses[QPair<LomiriNetworkInfo::NetworkMode, int>(mode, i)] = getNetworkStatus(mode, i);
        }

        watchNetworkStatus = true;
    } else if (signal == networkNameChangedSignal) {
        QList<LomiriNetworkInfo::NetworkMode> modes;
        modes << LomiriNetworkInfo::WlanMode << LomiriNetworkInfo::EthernetMode << LomiriNetworkInfo::BluetoothMode;
        networkNames.clear();
        Q_FOREACH (LomiriNetworkInfo::NetworkMode mode, modes) {
            int count = networkInterfaceCount(mode);
            for (int i = 0; i < count; ++i)
                networkNames[QPair<LomiriNetworkInfo::NetworkMode, int>(mode, i)] = getNetworkName(mode, i);
        }

        watchNetworkName = true;
    } else if (signal == currentNetworkModeChangedSignal) {
        currentMode = getCurrentNetworkMode();
        watchCurrentNetworkMode = true;
    } else {
        return;
    }

    if (!timer) {
        timer = new QTimer(this);
        timer->setInterval(2000);
        connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    }

    if (!timer->isActive())
        timer->start();
}

void LomiriNetworkInfoPrivate::disconnectNotify(const QMetaMethod &signal)
{
#if !defined(QT_NO_OFONO)
    if (!QOfonoWrapper::isOfonoAvailable() || !ofonoWrapper)
        return;

    {
        QMetaMethod sourceSignal = proxyToSourceSignal(signal, ofonoWrapper);
        disconnect(ofonoWrapper, sourceSignal, this, signal);
    }
#endif

    static const QMetaMethod currentNetworkModeChangedSignal = QMetaMethod::fromSignal(&LomiriNetworkInfoPrivate::currentNetworkModeChanged);
    static const QMetaMethod networkInterfaceCountChangedSignal = QMetaMethod::fromSignal(&LomiriNetworkInfoPrivate::networkInterfaceCountChanged);
    static const QMetaMethod networkNameChangedSignal = QMetaMethod::fromSignal(&LomiriNetworkInfoPrivate::networkNameChanged);
    static const QMetaMethod networkSignalStrengthChangedSignal = QMetaMethod::fromSignal(&LomiriNetworkInfoPrivate::networkSignalStrengthChanged);
    static const QMetaMethod networkStatusChangedSignal = QMetaMethod::fromSignal(&LomiriNetworkInfoPrivate::networkStatusChanged);

    if (signal == networkInterfaceCountChangedSignal
            && !watchNetworkStatus && !watchNetworkName && !watchNetworkSignalStrength ) {
#if !defined(QT_NO_UDEV)
        if (udevNotifier) {
            udevNotifier->setEnabled(false);
        }
        watchNetworkInterfaceCount = false;
        return;
#endif // QT_NO_UDEV
        watchNetworkInterfaceCount = false;
    } else if (signal == networkSignalStrengthChangedSignal) {
        watchNetworkSignalStrength = false;
    } else if ((!watchCurrentNetworkMode) && (signal == networkStatusChangedSignal)) {
        watchNetworkStatus = false;
    } else if (signal == networkNameChangedSignal) {
        watchNetworkName = false;
    } else if (signal == currentNetworkModeChangedSignal) {
        watchCurrentNetworkMode = false;
    } else {
        return;
    }

    if (!watchNetworkInterfaceCount && !watchNetworkSignalStrength && !watchNetworkStatus && !watchNetworkName && !watchCurrentNetworkMode)
        timer->stop();
}

#if !defined(QT_NO_UDEV)
void LomiriNetworkInfoPrivate::onUdevChanged()
{
    struct udev_device *udevDevice = udev_monitor_receive_device(udevMonitor);
    if (!udevDevice)
        return;

    if (0 != strcmp(udev_device_get_subsystem(udevDevice), "net"))
        return;

    QString sysname(QString::fromLocal8Bit(udev_device_get_sysname(udevDevice)));
    if (watchNetworkInterfaceCount) {
        if (sysname.startsWith(QLatin1String("eth"))
                || sysname.startsWith(QLatin1String("usb"))
                || sysname.startsWith(QLatin1String("enp"))) {
            if (0 == strcmp(udev_device_get_action(udevDevice), "add"))
                ++networkInterfaceCounts[LomiriNetworkInfo::EthernetMode];
            else if (0 == strcmp(udev_device_get_action(udevDevice), "remove"))
                --networkInterfaceCounts[LomiriNetworkInfo::EthernetMode];
            Q_EMIT networkInterfaceCountChanged(LomiriNetworkInfo::EthernetMode,
                                                networkInterfaceCounts[LomiriNetworkInfo::EthernetMode]);
        } else if (sysname.startsWith(QLatin1String("wlan")) || sysname.startsWith(QLatin1String("wlp"))) {
            if (0 == strcmp(udev_device_get_action(udevDevice), "add"))
                ++networkInterfaceCounts[LomiriNetworkInfo::WlanMode];
            else if (0 == strcmp(udev_device_get_action(udevDevice), "remove"))
                --networkInterfaceCounts[LomiriNetworkInfo::WlanMode];
            Q_EMIT networkInterfaceCountChanged(LomiriNetworkInfo::WlanMode,
                                                networkInterfaceCounts[LomiriNetworkInfo::WlanMode]);
        }
    }

    udev_device_unref(udevDevice);
}
#endif // QT_NO_UDEV

void LomiriNetworkInfoPrivate::onTimeout()
{
#if defined(QT_NO_UDEV)
    if (watchNetworkInterfaceCount) {
        QList<LomiriNetworkInfo::NetworkMode> modes;
        modes << LomiriNetworkInfo::WlanMode << LomiriNetworkInfo::EthernetMode << LomiriNetworkInfo::BluetoothMode;
        Q_FOREACH (LomiriNetworkInfo::NetworkMode mode, modes) {
            int value = getNetworkInterfaceCount(mode);
            if (networkInterfaceCounts.value(mode) != value) {
                networkInterfaceCounts[mode] = value;
                Q_EMIT networkInterfaceCountChanged(mode, value);
            }
        }
    }
#endif // QT_NO_UDEV

    if (!watchNetworkSignalStrength && !watchNetworkStatus && !watchNetworkName && !watchCurrentNetworkMode)
        return;

    QList<LomiriNetworkInfo::NetworkMode> modes;
    modes << LomiriNetworkInfo::WlanMode << LomiriNetworkInfo::EthernetMode << LomiriNetworkInfo::BluetoothMode;
    Q_FOREACH (LomiriNetworkInfo::NetworkMode mode, modes) {
        int count = networkInterfaceCount(mode);
        for (int i = 0; i < count; ++i) {
            if (watchNetworkSignalStrength) {
                int value = getNetworkSignalStrength(mode, i);
                QPair<LomiriNetworkInfo::NetworkMode, int> key(mode, i);
                if (networkSignalStrengths.value(key) != value) {
                    networkSignalStrengths[key] = value;
                    Q_EMIT networkSignalStrengthChanged(mode, i, value);
                }
            }

            if (watchNetworkStatus) {
                LomiriNetworkInfo::NetworkStatus value = getNetworkStatus(mode, i);
                QPair<LomiriNetworkInfo::NetworkMode, int> key(mode, i);
                if (networkStatuses.value(key) != value) {
                    networkStatuses[key] = value;
                    Q_EMIT networkStatusChanged(mode, i, value);
                }
            }

            if (watchNetworkName) {
                QString value = getNetworkName(mode, i);
                QPair<LomiriNetworkInfo::NetworkMode, int> key(mode, i);
                if (networkNames.value(key) != value) {
                    networkNames[key] = value;
                    Q_EMIT networkNameChanged(mode, i, value);
                }
            }
        }
    }

    if (watchCurrentNetworkMode) {
        LomiriNetworkInfo::NetworkMode value = getCurrentNetworkMode();
        if (currentMode != value) {
            currentMode = value;
            Q_EMIT currentNetworkModeChanged(value);
        }
    }

}

int LomiriNetworkInfoPrivate::getNetworkInterfaceCount(LomiriNetworkInfo::NetworkMode mode)
{
    switch (mode) {
    case LomiriNetworkInfo::WlanMode:
        return QDir(*NETWORK_SYSFS_PATH()).entryList(*WLAN_MASK()).size();

    case LomiriNetworkInfo::EthernetMode:
        return QDir(*NETWORK_SYSFS_PATH()).entryList(*ETHERNET_MASK()).size();

    case LomiriNetworkInfo::BluetoothMode: {
        int count = -1;
#if !defined(QT_NO_BLUEZ)
        int ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
        if (ctl < 0)
            return count;
        struct hci_dev_list_req *deviceList = (struct hci_dev_list_req *)malloc(HCI_MAX_DEV * sizeof(struct hci_dev_req) + sizeof(uint16_t));
        deviceList->dev_num = HCI_MAX_DEV;
        if (ioctl(ctl, HCIGETDEVLIST, deviceList) == 0)
            count = deviceList->dev_num;
        free(deviceList);
        close(ctl);
#endif // QT_NO_BLUEZ
        return count;
    }

    case LomiriNetworkInfo::GsmMode:
    case LomiriNetworkInfo::CdmaMode:
    case LomiriNetworkInfo::WcdmaMode:
    case LomiriNetworkInfo::LteMode:
    case LomiriNetworkInfo::TdscdmaMode:
#if !defined(QT_NO_OFONO)
        if (QOfonoWrapper::isOfonoAvailable()) {
            if (!ofonoWrapper)
                ofonoWrapper = new QOfonoWrapper(this);
            return ofonoWrapper->allModems().size();
        }
#endif

//    case LomiriNetworkInfo::WimaxMode:
    default:
        return -1;
    };
}

int LomiriNetworkInfoPrivate::getNetworkSignalStrength(LomiriNetworkInfo::NetworkMode mode, int interface)
{
    switch (mode) {
    case LomiriNetworkInfo::WlanMode: {
        QFile file(QString(QStringLiteral("/proc/net/wireless")));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return -1;

        QTextStream in(&file);
        QString interfaceName = interfaceForMode(LomiriNetworkInfo::WlanMode, interface).name();

        QStringList lines = in.readAll().split(QStringLiteral("\n"));
        for (int i = 0; i < lines.size(); i++) {
            QString line = lines.at(i);
            if (!line.isNull() && line.left(6).contains(interfaceName)) {
                // A typical wireless received signal power over a network falls into the range of (-120, -20),
                // we shift the dbm value, which is read from the field "Quality - level" in "/proc/net/wireless",
                // from (-120, -20) to (0, 100) by adding 120. In case of outliers, we restrict them to the
                // corresponding boundary value.
                QString token = line.section(QString(QStringLiteral(" ")), 3, 3, QString::SectionSkipEmpty).simplified();
                token.chop(1);
                bool ok;
                int signalStrength = token.toInt(&ok);
                if (ok) {
                    signalStrength += 120;
                    if (signalStrength > 100)
                        signalStrength = 100;
                    else if (signalStrength < 0)
                        signalStrength = 0;
                    return signalStrength;
                } else {
                    return -1;
                }
            }
        }

        break;
    }

    case LomiriNetworkInfo::EthernetMode:
        if (networkStatus(LomiriNetworkInfo::EthernetMode, interface) == LomiriNetworkInfo::HomeNetwork)
            return 100;
        else
            return -1;

    case LomiriNetworkInfo::GsmMode:
    case LomiriNetworkInfo::CdmaMode:
    case LomiriNetworkInfo::WcdmaMode:
    case LomiriNetworkInfo::LteMode:
    case LomiriNetworkInfo::TdscdmaMode:
#if !defined(QT_NO_OFONO)
        if (QOfonoWrapper::isOfonoAvailable()) {
            if (!ofonoWrapper)
                ofonoWrapper = new QOfonoWrapper(this);
            QStringList modems = ofonoWrapper->allModems();
            if (interface < modems.size()) {
                QString modem = ofonoWrapper->allModems().at(interface);
                if (!modem.isEmpty())
                    return ofonoWrapper->signalStrength(modem);
            }
        }
#endif
        break;

    case LomiriNetworkInfo::BluetoothMode: {
        int signalStrength = -1;
#if !defined(QT_NO_BLUEZ)
        int ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
        if (ctl < 0)
            break;
        struct hci_dev_list_req *deviceList = (struct hci_dev_list_req *)malloc(HCI_MAX_DEV * sizeof(struct hci_dev_req) + sizeof(uint16_t));
        deviceList->dev_num = HCI_MAX_DEV;
        if (ioctl(ctl, HCIGETDEVLIST, deviceList) == 0) {
            int count = deviceList->dev_num;
            if (interface < count) {
                signalStrength = 0; // Valid interface

                struct hci_conn_list_req *connectionList = (struct hci_conn_list_req *)malloc(sizeof(struct hci_conn_info) + sizeof(struct hci_conn_list_req));
                connectionList->dev_id = (deviceList->dev_req + interface)->dev_id;
                connectionList->conn_num = 1;
                if (ioctl(ctl, HCIGETCONNLIST, connectionList) == 0) {
                    if (connectionList->conn_num == 1) {
                        int fd = hci_open_dev((deviceList->dev_req + interface)->dev_id);
                        if (fd > 0) {
                            struct hci_conn_info_req *connectionInfo = (struct hci_conn_info_req *)malloc(sizeof(struct hci_conn_info_req) + sizeof(struct hci_conn_info));
                            bacpy(&connectionInfo->bdaddr, &connectionList->conn_info->bdaddr);
                            connectionInfo->type = ACL_LINK;
                            if (ioctl(fd, HCIGETCONNINFO, connectionInfo) == 0) {
                                uint8_t linkQuality;
                                if (hci_read_link_quality(fd, htobs(connectionInfo->conn_info->handle), &linkQuality, 0) == 0)
                                    signalStrength = linkQuality * 100 / 255;
                            }
                            free(connectionInfo);
                        }
                    }
                }
                free (connectionList);
            }
        }
        free(deviceList);
        close(ctl);
#endif // QT_NO_BLUEZ
        return signalStrength;
    }

//    case LomiriNetworkInfo::WimaxMode:
    default:
        break;
    };

    return -1;
}

LomiriNetworkInfo::NetworkMode LomiriNetworkInfoPrivate::getCurrentNetworkMode()
{
    // TODO multiple-interface support
    if (networkStatus(LomiriNetworkInfo::EthernetMode, 0) == LomiriNetworkInfo::HomeNetwork)
        return LomiriNetworkInfo::EthernetMode;
    else if (networkStatus(LomiriNetworkInfo::WlanMode, 0) == LomiriNetworkInfo::HomeNetwork)
        return LomiriNetworkInfo::WlanMode;
    else if (networkStatus(LomiriNetworkInfo::BluetoothMode, 0) == LomiriNetworkInfo::HomeNetwork)
        return LomiriNetworkInfo::BluetoothMode;
    else if (networkStatus(LomiriNetworkInfo::WimaxMode, 0) == LomiriNetworkInfo::HomeNetwork)
        return LomiriNetworkInfo::WimaxMode;
    else if (networkStatus(LomiriNetworkInfo::LteMode, 0) == LomiriNetworkInfo::HomeNetwork)
        return LomiriNetworkInfo::LteMode;
    else if (networkStatus(LomiriNetworkInfo::WcdmaMode, 0) == LomiriNetworkInfo::HomeNetwork)
        return LomiriNetworkInfo::WcdmaMode;
    else if (networkStatus(LomiriNetworkInfo::CdmaMode, 0) == LomiriNetworkInfo::HomeNetwork)
        return LomiriNetworkInfo::CdmaMode;
    else if (networkStatus(LomiriNetworkInfo::GsmMode, 0) == LomiriNetworkInfo::HomeNetwork)
        return LomiriNetworkInfo::GsmMode;
    else if (networkStatus(LomiriNetworkInfo::TdscdmaMode, 0) == LomiriNetworkInfo::HomeNetwork)
        return LomiriNetworkInfo::TdscdmaMode;
    else if (networkStatus(LomiriNetworkInfo::WimaxMode, 0) == LomiriNetworkInfo::Roaming)
        return LomiriNetworkInfo::WimaxMode;
    else if (networkStatus(LomiriNetworkInfo::LteMode, 0) == LomiriNetworkInfo::Roaming)
        return LomiriNetworkInfo::LteMode;
    else if (networkStatus(LomiriNetworkInfo::WcdmaMode, 0) == LomiriNetworkInfo::Roaming)
        return LomiriNetworkInfo::WcdmaMode;
    else if (networkStatus(LomiriNetworkInfo::CdmaMode, 0) == LomiriNetworkInfo::Roaming)
        return LomiriNetworkInfo::CdmaMode;
    else if (networkStatus(LomiriNetworkInfo::GsmMode, 0) == LomiriNetworkInfo::Roaming)
        return LomiriNetworkInfo::GsmMode;
    else if (networkStatus(LomiriNetworkInfo::TdscdmaMode, 0) == LomiriNetworkInfo::Roaming)
        return LomiriNetworkInfo::TdscdmaMode;
    else
        return LomiriNetworkInfo::UnknownMode;
}

LomiriNetworkInfo::NetworkStatus LomiriNetworkInfoPrivate::getNetworkStatus(LomiriNetworkInfo::NetworkMode mode, int interface)
{
    switch (mode) {
    case LomiriNetworkInfo::WlanMode: {
        if (interface < networkInterfaceCount(LomiriNetworkInfo::WlanMode)) {
            const QString fileName = QDir(*NETWORK_SYSFS_PATH()).entryList(*WLAN_MASK()).at(interface);
            QFile carrier(*NETWORK_SYSFS_PATH() + fileName + QStringLiteral("/carrier"));
            if (carrier.open(QIODevice::ReadOnly)) {
                char state;
                if ((carrier.read(&state, 1) == 1) &&
                        (state == '1')) {
                    return LomiriNetworkInfo::HomeNetwork;
                }
            }
        }
        return LomiriNetworkInfo::NoNetworkAvailable;
    }

    case LomiriNetworkInfo::EthernetMode: {
        if (interface < networkInterfaceCount(LomiriNetworkInfo::EthernetMode)) {
            const QString fileName = QDir(*NETWORK_SYSFS_PATH()).entryList(*ETHERNET_MASK()).at(interface);
            QFile carrier(*NETWORK_SYSFS_PATH() + fileName + QStringLiteral("/carrier"));
            if (carrier.open(QIODevice::ReadOnly)) {
                char state;
                if ((carrier.read(&state, 1) == 1) && (state == '1'))
                    return LomiriNetworkInfo::HomeNetwork;
            }
        }
        return LomiriNetworkInfo::NoNetworkAvailable;
    }

    case LomiriNetworkInfo::BluetoothMode: {
        LomiriNetworkInfo::NetworkStatus status = LomiriNetworkInfo::UnknownStatus;

#if !defined(QT_NO_BLUEZ)
        int ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
        if (ctl < 0)
            break;
        struct hci_dev_list_req *deviceList = (struct hci_dev_list_req *)malloc(HCI_MAX_DEV * sizeof(struct hci_dev_req) + sizeof(uint16_t));
        deviceList->dev_num = HCI_MAX_DEV;
        if (ioctl(ctl, HCIGETDEVLIST, deviceList) == 0) {
            int count = deviceList->dev_num;
            if (interface < count) {
                status = LomiriNetworkInfo::NoNetworkAvailable; // Valid interface, so either not connected or connected
                                                           // TODO add support for searching and denied
                struct hci_conn_list_req *connectionList = (struct hci_conn_list_req *)malloc(sizeof(struct hci_conn_info) + sizeof(struct hci_conn_list_req));
                connectionList->dev_id = (deviceList->dev_req + interface)->dev_id;
                connectionList->conn_num = 1;
                if (ioctl(ctl, HCIGETCONNLIST, connectionList) == 0) {
                    if (connectionList->conn_num == 1)
                        status = LomiriNetworkInfo::HomeNetwork;
                }
                free (connectionList);
            }
        }
        free(deviceList);
        close(ctl);
#endif // QT_NO_BLUEZ

        return status;
    }

    case LomiriNetworkInfo::GsmMode:
    case LomiriNetworkInfo::CdmaMode:
    case LomiriNetworkInfo::WcdmaMode:
    case LomiriNetworkInfo::LteMode:
    case LomiriNetworkInfo::TdscdmaMode:
#if !defined(QT_NO_OFONO)
        if (QOfonoWrapper::isOfonoAvailable()) {
            if (!ofonoWrapper)
                ofonoWrapper = new QOfonoWrapper(this);
            QStringList modems = ofonoWrapper->allModems();
            if (interface < modems.size()) {
                QString modem = ofonoWrapper->allModems().at(interface);
                if (!modem.isEmpty())
                    return ofonoWrapper->networkStatus(modem);
            }
    }
#endif
        break;

//    case LomiriNetworkInfo::WimaxMode:
    default:
        break;
    };

    return LomiriNetworkInfo::UnknownStatus;
}

QString LomiriNetworkInfoPrivate::getNetworkName(LomiriNetworkInfo::NetworkMode mode, int interface)
{
    switch (mode) {
    case LomiriNetworkInfo::WlanMode: {
        if (interface < networkInterfaceCount(LomiriNetworkInfo::WlanMode)) {
            int sock = socket(PF_INET, SOCK_DGRAM, 0);
            if (sock > 0) {
                char buffer[IW_ESSID_MAX_SIZE + 1];
                iwreq iwInfo;

                iwInfo.u.essid.pointer = (caddr_t)&buffer;
                iwInfo.u.essid.length = IW_ESSID_MAX_SIZE + 1;
                iwInfo.u.essid.flags = 0;
                for (int i = 0; i < WLAN_MASK()->count(); i++) {
                    const QString fileName = QDir(*NETWORK_SYSFS_PATH()).entryList(*WLAN_MASK()).at(interface);
                    strncpy(iwInfo.ifr_name, fileName.toLocal8Bit().constData(), IFNAMSIZ);
                    if (ioctl(sock, SIOCGIWESSID, &iwInfo) == 0) {
                        close(sock);
                        return QString::fromLatin1((const char *)iwInfo.u.essid.pointer);
                    } else {
                        qDebug() << "ioctl failed";
                    }

                    close(sock);

                }
            }
        }
        break;
    }

    case LomiriNetworkInfo::EthernetMode: {
        // TODO multiple-interface support
        char domainName[64];
        if (getdomainname(domainName, 64) == 0) {
            if (strcmp(domainName, "(none)") != 0)
                return QString::fromLatin1(domainName);
            else
                return QStringLiteral("Unknown");
        }
        break;
    }

    case LomiriNetworkInfo::BluetoothMode: {
#if !defined(QT_NO_BLUEZ)
        int ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
        if (ctl < 0)
            break;
        struct hci_dev_list_req *deviceList = (struct hci_dev_list_req *)malloc(HCI_MAX_DEV * sizeof(struct hci_dev_req) + sizeof(uint16_t));
        deviceList->dev_num = HCI_MAX_DEV;
        QString networkName;
        if (ioctl(ctl, HCIGETDEVLIST, deviceList) == 0) {
            int count = deviceList->dev_num;
            if (interface < count) {
                int fd = hci_open_dev((deviceList->dev_req + interface)->dev_id);
                if (fd > 0) {
                    char name[249];
                    if (hci_read_local_name(fd, sizeof(name), name, 0) == 0)
                        networkName = QString::fromLatin1(name);
                }
            }
        }
        free(deviceList);
        close(ctl);
        return networkName;
#endif // QT_NO_BLUEZ
        break;
    }

    case LomiriNetworkInfo::GsmMode:
    case LomiriNetworkInfo::CdmaMode:
    case LomiriNetworkInfo::WcdmaMode:
    case LomiriNetworkInfo::LteMode:
    case LomiriNetworkInfo::TdscdmaMode:
#if !defined(QT_NO_OFONO)
        if (QOfonoWrapper::isOfonoAvailable()) {
            if (!ofonoWrapper)
                ofonoWrapper = new QOfonoWrapper(this);
            QStringList modems = ofonoWrapper->allModems();
            if (interface < modems.size()) {
                QString modem = ofonoWrapper->allModems().at(interface);
                if (!modem.isEmpty())
                    return ofonoWrapper->operatorName(modem);
            }
        }
#endif
        break;

//    case LomiriNetworkInfo::WimaxMode:
    default:
        break;
    };

    return QString();
}

QT_END_NAMESPACE
