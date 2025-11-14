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

#ifndef QDEVICEINFO_H
#define QDEVICEINFO_H

#include "lomirisysteminfoglobal.h"
#include <QtCore/qobject.h>

QT_BEGIN_NAMESPACE

class LomiriDeviceInfoPrivate;

class LOMIRISYSTEMINFO_EXPORT LomiriDeviceInfo : public QObject
{
    Q_OBJECT

    Q_ENUMS(Feature)
    Q_ENUMS(LockType)
    Q_ENUMS(ThermalState)
    Q_ENUMS(Version)

    Q_FLAGS(LockType LockTypeFlags)

    Q_PROPERTY(LockTypeFlags activatedLocks READ activatedLocks NOTIFY activatedLocksChanged)
    Q_PROPERTY(LockTypeFlags enabledLocks READ enabledLocks NOTIFY enabledLocksChanged)
    Q_PROPERTY(ThermalState thermalState READ thermalState NOTIFY thermalStateChanged)
    Q_PROPERTY(bool currentBluetoothPowerState READ currentBluetoothPowerState NOTIFY bluetoothStateChanged)

public:
    enum Feature {
        BluetoothFeature = 0,
        CameraFeature,
        FmRadioFeature,
        FmTransmitterFeature,
        InfraredFeature,
        LedFeature,
        MemoryCardFeature,
        UsbFeature,
        VibrationFeature,
        WlanFeature,
        SimFeature,
        PositioningFeature,
        VideoOutFeature,
        HapticsFeature,
        NfcFeature
    };

    enum LockType {
        NoLock = 0,
        PinLock = 0x0000001,
        TouchOrKeyboardLock = 0x0000002,
        UnknownLock = 0x0000004
    };
    Q_DECLARE_FLAGS(LockTypeFlags, LockType)

    enum ThermalState {
        UnknownThermal = 0,
        NormalThermal,
        WarningThermal,
        AlertThermal,
        ErrorThermal
    };

    enum Version {
        Os = 0,
        Firmware
    };

    explicit LomiriDeviceInfo(QObject *parent = Q_NULLPTR);
    virtual ~LomiriDeviceInfo();

    LomiriDeviceInfo::LockTypeFlags activatedLocks() const;
    LomiriDeviceInfo::LockTypeFlags enabledLocks() const;
    LomiriDeviceInfo::ThermalState thermalState() const;

    Q_INVOKABLE bool hasFeature(LomiriDeviceInfo::Feature feature) const;
    Q_INVOKABLE int imeiCount() const;
    Q_INVOKABLE QString imei(int interfaceNumber) const;
    Q_INVOKABLE QString manufacturer() const;
    Q_INVOKABLE QString model() const;
    Q_INVOKABLE QString productName() const;
    Q_INVOKABLE QString uniqueDeviceID() const;
    Q_INVOKABLE QString version(LomiriDeviceInfo::Version type) const;
    Q_INVOKABLE QString operatingSystemName() const;
    Q_INVOKABLE QString boardName() const;

    bool currentBluetoothPowerState();

Q_SIGNALS:
    void activatedLocksChanged(LomiriDeviceInfo::LockTypeFlags types);
    void enabledLocksChanged(LomiriDeviceInfo::LockTypeFlags types);
    void thermalStateChanged(LomiriDeviceInfo::ThermalState state);
    void bluetoothStateChanged(bool on);

protected:
    void connectNotify(const QMetaMethod &signal) override;
    void disconnectNotify(const QMetaMethod &signal) override;

private:
    Q_DISABLE_COPY(LomiriDeviceInfo)
    LomiriDeviceInfoPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(LomiriDeviceInfo)
};

QT_END_NAMESPACE

#endif // QDEVICEINFO_H
