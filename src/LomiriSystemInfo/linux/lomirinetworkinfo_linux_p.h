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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#ifndef QNETWORKINFO_LINUX_P_H
#define QNETWORKINFO_LINUX_P_H

#include <lomirinetworkinfo.h>

#include <QtCore/qmap.h>

#if !defined(QT_NO_UDEV)
struct udev;
struct udev_monitor;
#endif // QT_NO_UDEV

QT_BEGIN_NAMESPACE

class QTimer;

#if !defined(QT_NO_OFONO)
class QOfonoWrapper;
#endif

#if !defined(QT_NO_UDEV)
class QSocketNotifier;
#endif // QT_NO_UDEV

class LomiriNetworkInfoPrivate : public QObject
{
    Q_OBJECT

public:
    LomiriNetworkInfoPrivate(LomiriNetworkInfo *parent = 0);
    ~LomiriNetworkInfoPrivate();

    int networkInterfaceCount(LomiriNetworkInfo::NetworkMode mode);
    int networkSignalStrength(LomiriNetworkInfo::NetworkMode mode, int interface);
    LomiriNetworkInfo::CellDataTechnology currentCellDataTechnology(int interface);
    LomiriNetworkInfo::NetworkMode currentNetworkMode();
    LomiriNetworkInfo::NetworkStatus networkStatus(LomiriNetworkInfo::NetworkMode mode, int interface);
#ifndef QT_NO_NETWORKINTERFACE
    QNetworkInterface interfaceForMode(LomiriNetworkInfo::NetworkMode mode, int interface);
#endif // QT_NO_NETWORKINTERFACE
    QString cellId(int interface);
    QString currentMobileCountryCode(int interface);
    QString currentMobileNetworkCode(int interface);
    QString homeMobileCountryCode(int interface);
    QString homeMobileNetworkCode(int interface);
    QString imsi(int interface);
    QString locationAreaCode(int interface);
    QString macAddress(LomiriNetworkInfo::NetworkMode mode, int interface);
    QString networkName(LomiriNetworkInfo::NetworkMode mode, int interface);

Q_SIGNALS:
    void cellIdChanged(int interface, const QString &id);
    void currentCellDataTechnologyChanged(int interface, LomiriNetworkInfo::CellDataTechnology tech);
    void currentMobileCountryCodeChanged(int interface, const QString &mcc);
    void currentMobileNetworkCodeChanged(int interface, const QString &mnc);
    void currentNetworkModeChanged(LomiriNetworkInfo::NetworkMode mode);
    void locationAreaCodeChanged(int interface, const QString &lac);
    void networkInterfaceCountChanged(LomiriNetworkInfo::NetworkMode mode, int count);
    void networkNameChanged(LomiriNetworkInfo::NetworkMode mode, int interface, const QString &name);
    void networkSignalStrengthChanged(LomiriNetworkInfo::NetworkMode mode, int interface, int strength);
    void networkStatusChanged(LomiriNetworkInfo::NetworkMode mode, int interface, LomiriNetworkInfo::NetworkStatus status);

protected:
    void connectNotify(const QMetaMethod &signal);
    void disconnectNotify(const QMetaMethod &signal);

private Q_SLOTS:
#if !defined(QT_NO_UDEV)
    void onUdevChanged();
#endif // QT_NO_UDEV

    void onTimeout();

private:
    LomiriNetworkInfo * const q_ptr;
    Q_DECLARE_PUBLIC(LomiriNetworkInfo)

    int getNetworkInterfaceCount(LomiriNetworkInfo::NetworkMode mode);
    int getNetworkSignalStrength(LomiriNetworkInfo::NetworkMode mode, int interface);
    LomiriNetworkInfo::NetworkMode getCurrentNetworkMode();
    LomiriNetworkInfo::NetworkStatus getNetworkStatus(LomiriNetworkInfo::NetworkMode mode, int interface);
    QString getNetworkName(LomiriNetworkInfo::NetworkMode mode, int interface);

    bool watchCurrentNetworkMode;
    bool watchNetworkInterfaceCount;
    bool watchNetworkSignalStrength;
    bool watchNetworkStatus;
    bool watchNetworkName;
    LomiriNetworkInfo::NetworkMode currentMode;
    QMap<LomiriNetworkInfo::NetworkMode, int> networkInterfaceCounts;
    QMap<QPair<LomiriNetworkInfo::NetworkMode, int>, int> networkSignalStrengths;
    QMap<QPair<LomiriNetworkInfo::NetworkMode, int>, LomiriNetworkInfo::NetworkStatus> networkStatuses;
    QMap<QPair<LomiriNetworkInfo::NetworkMode, int>, QString> networkNames;

    QTimer *timer;

#if !defined(QT_NO_OFONO)
    QOfonoWrapper *ofonoWrapper;
#endif

#if !defined(QT_NO_UDEV)
    QSocketNotifier *udevNotifier;
    struct udev *udevHandle;
    struct udev_monitor *udevMonitor;
#endif // QT_NO_UDEV
};

QT_END_NAMESPACE

#endif // QNETWORKINFO_LINUX_P_H
