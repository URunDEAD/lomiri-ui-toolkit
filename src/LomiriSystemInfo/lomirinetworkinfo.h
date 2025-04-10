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

#ifndef QNETWORKINFO_H
#define QNETWORKINFO_H

#include "lomirisysteminfoglobal.h"
#include <QtCore/qobject.h>
#include <QtNetwork/qnetworkinterface.h>

QT_BEGIN_NAMESPACE

class LomiriNetworkInfoPrivate;

class LOMIRISYSTEMINFO_EXPORT LomiriNetworkInfo : public QObject
{
    Q_OBJECT

public:
    enum CellDataTechnology {
        UnknownDataTechnology = 0,
        GprsDataTechnology,
        EdgeDataTechnology,
        UmtsDataTechnology,
        HspaDataTechnology
    };

    enum NetworkMode {
        UnknownMode = 0,
        GsmMode,
        CdmaMode,
        WcdmaMode,
        WlanMode,
        EthernetMode,
        BluetoothMode,
        WimaxMode,
        LteMode,
        TdscdmaMode
    };

    enum NetworkStatus {
        UnknownStatus = 0,
        NoNetworkAvailable,
        EmergencyOnly,
        Searching,
        Busy,
        Denied,
        HomeNetwork,
        Roaming
        // ,Connected //desktop
    };

    explicit LomiriNetworkInfo(QObject *parent = Q_NULLPTR);
    virtual ~LomiriNetworkInfo();

    int networkInterfaceCount(LomiriNetworkInfo::NetworkMode mode) const;
    int networkSignalStrength(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice) const;
    LomiriNetworkInfo::CellDataTechnology currentCellDataTechnology(int interfaceDevice) const;
    LomiriNetworkInfo::NetworkMode currentNetworkMode() const;
    LomiriNetworkInfo::NetworkStatus networkStatus(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice) const;
#ifndef QT_NO_NETWORKINTERFACE
    QNetworkInterface interfaceForMode(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice) const;
#endif // QT_NO_NETWORKINTERFACE
    QString cellId(int interfaceDevice) const;
    QString currentMobileCountryCode(int interfaceDevice) const;
    QString currentMobileNetworkCode(int interfaceDevice) const;
    QString homeMobileCountryCode(int interfaceDevice) const;
    QString homeMobileNetworkCode(int interfaceDevice) const;
    QString imsi(int interfaceDevice) const;
    QString locationAreaCode(int interfaceDevice) const;
    QString macAddress(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice) const;
    QString networkName(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice) const;

Q_SIGNALS:
    void cellIdChanged(int interfaceDevice, const QString &id);
    void currentCellDataTechnologyChanged(int interfaceDevice, LomiriNetworkInfo::CellDataTechnology tech);
    void currentMobileCountryCodeChanged(int interfaceDevice, const QString &mcc);
    void currentMobileNetworkCodeChanged(int interfaceDevice, const QString &mnc);
    void currentNetworkModeChanged(LomiriNetworkInfo::NetworkMode mode);
    void locationAreaCodeChanged(int interfaceDevice, const QString &lac);
    void networkInterfaceCountChanged(LomiriNetworkInfo::NetworkMode mode, int count);
    void networkNameChanged(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice, const QString &name);
    void networkSignalStrengthChanged(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice, int strength);
    void networkStatusChanged(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice, LomiriNetworkInfo::NetworkStatus status);

protected:
    void connectNotify(const QMetaMethod &signal) override;
    void disconnectNotify(const QMetaMethod &signal) override;

private:
    Q_DISABLE_COPY(LomiriNetworkInfo)
    LomiriNetworkInfoPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(LomiriNetworkInfo)
};

QT_END_NAMESPACE

#endif // QNETWORKINFO_H
