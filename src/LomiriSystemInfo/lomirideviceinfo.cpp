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

#include "lomirideviceinfo.h"

#include "linux/lomirideviceinfo_linux_p.h"

#include <QtCore/qmetaobject.h>

QT_BEGIN_NAMESPACE

/*!
    \class LomiriDeviceInfo
    \inmodule QtSystemInfo
    \brief The LomiriDeviceInfo class provides various information of the system.
    \ingroup systeminfo
*/

/*!
    \enum LomiriDeviceInfo::Feature
    This enum describes the features of the device.

    \value BluetoothFeature      Bluetooth feature.
    \value CameraFeature         Camera feature.
    \value FmRadioFeature        Frequency modulation (FM) radio feature.
    \value FmTransmitterFeature  Frequency modulation (FM) radio transmitter feature.
    \value InfraredFeature       Infrared communication feature.
    \value LedFeature            Light-emitting diode (LED) feature.
    \value MemoryCardFeature     Memory card feature.
    \value UsbFeature            Universal system bus (USB) feature.
    \value VibrationFeature      Vibration feature.
    \value WlanFeature           Wireless local area network (WLAN) feature.
    \value SimFeature            Subscriber identity module (SIM) feature.
    \value PositioningFeature    Positioning feature, e.g. Global Positioning System (GPS).
    \value VideoOutFeature       Video out feature.
    \value HapticsFeature        Haptics feature, the platform can provide audio and/or visual and/or vibration feedback.
    \value NfcFeature            Near Field Communication (NFC) feature.
*/

/*!
    \enum LomiriDeviceInfo::LockType
    This enum describes lock type on the device.

    \value NoLock               No lock.
    \value PinLock              Device can be locked by PIN code or password.
    \value TouchOrKeyboardLock  Device can be locked by touch or keyboard.
    \value UnknownLock          lock types requested but no result received yet.
*/

/*!
    \enum LomiriDeviceInfo::ThermalState
    This enum describes the thermal state:

    \value UnknownThermal   The thermal state is unknown.
    \value NormalThermal    The thermal state is normal.
    \value WarningThermal   The thermal state is warning.
    \value AlertThermal     The thermal state is alert.
    \value ErrorThermal     The thermal state is error.
*/

/*!
    \enum LomiriDeviceInfo::Version
    This enum describes the version component.

    \value Os                    Operating system version. For Linux, it returns the version of the
                                 distribution if any.
    \value Firmware              Version of (flashable) system as a whole. For Linux, it's the version
                                 of the kernel.
*/

/*!
    \fn void LomiriDeviceInfo::activatedLocksChanged(LomiriDeviceInfo::LockTypeFlags types)

    This signal is emitted when the activated locks have changed to \a types.
*/

/*!
    \fn void LomiriDeviceInfo::enabledLocksChanged(LomiriDeviceInfo::LockTypeFlags types)

    This signal is emitted when the enabled locks have changed to \a types.
*/

/*!
    \fn void LomiriDeviceInfo::thermalStateChanged(LomiriDeviceInfo::ThermalState state)

    This signal is emitted when the thermal state has changed to \a state.
*/

/*!
    Constructs a LomiriDeviceInfo object with the given \a parent.
*/
LomiriDeviceInfo::LomiriDeviceInfo(QObject *parent)
    : QObject(parent)
    , d_ptr(new LomiriDeviceInfoPrivate(this))
{
}

/*!
    Destroys the object
*/
LomiriDeviceInfo::~LomiriDeviceInfo()
{
    delete d_ptr;
}

/*!
    \property LomiriDeviceInfo::activatedLocks
    \brief The activated lock types.

    The current activated lock types of the device. It suggests that these lock types are also
    enabled.
*/
LomiriDeviceInfo::LockTypeFlags LomiriDeviceInfo::activatedLocks() const
{
    return d_ptr->activatedLocks();
}

/*!
    \property LomiriDeviceInfo::enabledLocks
    \brief The enabled lock types.

    The current lock types that are enabled on the device. Note that it doesn't mean the device is
    currently locked.
*/
LomiriDeviceInfo::LockTypeFlags LomiriDeviceInfo::enabledLocks() const
{
    return d_ptr->enabledLocks();
}

/*!
    \property LomiriDeviceInfo::thermalState
    \brief The thermal state.

    The current thermal state of the device. If there are more than one thermal zone devices available,
    the state of the most critical one is reported.
*/
LomiriDeviceInfo::ThermalState LomiriDeviceInfo::thermalState() const
{
    return d_ptr->thermalState();
}

/*!
    Returns true if the \a feature is supported, otherwise false.
*/
bool LomiriDeviceInfo::hasFeature(LomiriDeviceInfo::Feature feature) const
{
    return d_ptr->hasFeature(feature);
}

/*!
    Returns the count of available International Mobile Equipment Identity (IMEI) of the device. In
    case of error, or the information is not available, -1 is returned.
*/
int LomiriDeviceInfo::imeiCount() const
{
    return d_ptr->imeiCount();
}

/*!
    Returns the International Mobile Equipment Identity (IMEI) of the given \a interfaceNumber on the device.
    In case of error, or the information is not available, an empty string is returned.
*/
QString LomiriDeviceInfo::imei(int interfaceNumber) const
{
    return d_ptr->imei(interfaceNumber);
}

/*!
    Returns the name of the manufacturer of this device, or the name of the vendor of the motherboard
    as a fallback. In case of error, or the information is not available, an empty string is returned.
*/
QString LomiriDeviceInfo::manufacturer() const
{
    return d_ptr->manufacturer();
}

/*!
    Returns the model information of the device, e.g. N8, or the CPU architect as a fallback. In case
    of error, or the information is not available, an empty string is returned.
*/
QString LomiriDeviceInfo::model() const
{
    return d_ptr->model();
}

/*!
    Returns the internal product name of the device, e.g. RM-774. In case of error, or the information
    is not available, an empty string is returned.

    For Linux, it returns the codename of the distribution if any.
*/
QString LomiriDeviceInfo::productName() const
{
    return d_ptr->productName();
}

/*!
    Returns a unique identifier for the device, or an empty string if on error or not available.
*/
QString LomiriDeviceInfo::uniqueDeviceID() const
{
    return d_ptr->uniqueDeviceID();
}

/*!
    Returns the version of \a type. In case of error, or the version is unknown, an empty string
    is returned.
*/
QString LomiriDeviceInfo::version(LomiriDeviceInfo::Version type) const
{
    return d_ptr->version(type);
}

/*!
    Returns the current bluetooth power state.
*/
bool LomiriDeviceInfo::currentBluetoothPowerState()
{
    return d_ptr->currentBluetoothPowerState();
}

/*!
    Returns the name for the operating system for the device, or an empty string if an error or not available.
*/
QString LomiriDeviceInfo::operatingSystemName() const
{
    return d_ptr->operatingSystemName();
}

/*!
    Returns the board name for the device, or an empty string if an error or not available.
*/
QString LomiriDeviceInfo::boardName() const
{
    return d_ptr->boardName();
}

extern QMetaMethod proxyToSourceSignal(const QMetaMethod &, QObject *);

/*!
    \internal
*/
void LomiriDeviceInfo::connectNotify(const QMetaMethod &signalSig)
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN) || defined(Q_OS_MAC)
    QMetaMethod sourceSignal = proxyToSourceSignal(signalSig, reinterpret_cast<QObject *>(d_ptr));
    connect(reinterpret_cast<QObject *>(d_ptr), sourceSignal, this, signalSig, Qt::UniqueConnection);
#else
    Q_UNUSED(signalSig)
#endif
}

/*!
    \internal
*/
void LomiriDeviceInfo::disconnectNotify(const QMetaMethod &signalSig)
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN) || defined(Q_OS_MAC)
    // We can only disconnect with the private implementation, when there is no receivers for the signal.
    if (isSignalConnected(signalSig))
        return;

    QMetaMethod sourceSignal = proxyToSourceSignal(signalSig, reinterpret_cast<QObject *>(d_ptr));
    disconnect(reinterpret_cast<QObject *>(d_ptr), sourceSignal, this, signalSig);
#else
    Q_UNUSED(signalSig)
#endif
}

QT_END_NAMESPACE
