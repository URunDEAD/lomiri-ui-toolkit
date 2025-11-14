/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd and/or its subsidiary(-ies).
** Copyright (C) 2018 BlackBerry Limited. All rights reserved.
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

#include "lomiribatteryinfo_linux_p.h"

#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qtimer.h>
#include <QtCore/qnumeric.h>

#if !defined(QT_NO_UDEV)
#include "lomiriudevwrapper_p.h"
#endif // QT_NO_UDEV

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC_WITH_ARGS(const QString, AC_ONLINE_SYSFS_PATH, (QLatin1String("/sys/class/power_supply/AC/online")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, BATTERY_SYSFS_PATH, (QLatin1String("/sys/class/power_supply/BAT%1/")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, POWER_SUPPLY_SYSFS_PATH, (QLatin1String("/sys/class/power_supply/")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, USB_PRESENT_SYSFS_PATH, (QLatin1String("/sys/class/power_supply/usb/present")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, USB_TYPE_SYSFS_PATH, (QLatin1String("/sys/class/power_supply/usb/type")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, USB0_PRESENT_SYSFS_PATH, (QLatin1String("/sys/class/power_supply/USB0/present")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, USB0_TYPE_SYSFS_PATH, (QLatin1String("/sys/class/power_supply/USB0/type")))

LomiriBatteryInfoPrivate::LomiriBatteryInfoPrivate(LomiriBatteryInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , watchIsValid(false)
    , forceWatchBatteryCount(false)
    , watchBatteryCount(false)
    , watchChargerType(false)
    , watchChargingState(false)
    , watchCurrentFlow(false)
    , watchRemainingCapacity(false)
    , watchRemainingChargingTime(false)
    , watchVoltage(false)
    , watchLevelStatus(false)
    , batteryCounts(-1)
    , index(0)
    , currentChargerType(LomiriBatteryInfo::UnknownCharger)
#if !defined(QT_NO_UDEV)
    , uDevWrapper(0)
#else
    , timer(0)
#endif // QT_NO_UDEV
{
}

LomiriBatteryInfoPrivate::LomiriBatteryInfoPrivate(int batteryIndex, LomiriBatteryInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , watchIsValid(false)
    , forceWatchBatteryCount(false)
    , watchBatteryCount(false)
    , watchChargerType(false)
    , watchChargingState(false)
    , watchCurrentFlow(false)
    , watchRemainingCapacity(false)
    , watchRemainingChargingTime(false)
    , watchVoltage(false)
    , watchLevelStatus(false)
    , batteryCounts(-1)
    , index(batteryIndex)
    , currentChargerType(LomiriBatteryInfo::UnknownCharger)
#if !defined(QT_NO_UDEV)
    , uDevWrapper(0)
#else
    , timer(0)
#endif // QT_NO_UDEV
{
}

LomiriBatteryInfoPrivate::~LomiriBatteryInfoPrivate()
{
#if defined(QT_NO_UDEV)
    delete timer;
#endif // QT_NO_UDEV
}

int LomiriBatteryInfoPrivate::batteryCount()
{
    if (!watchBatteryCount)
        return getBatteryCount();

    return batteryCounts;
}

int LomiriBatteryInfoPrivate::batteryIndex() const
{
    return index;
}

bool LomiriBatteryInfoPrivate::isValid()
{
    // valid if the index < total count.
    return (index >= 0) && (index < batteryCount());
}

void LomiriBatteryInfoPrivate::setBatteryIndex(int batteryIndex)
{
    if (index != batteryIndex) {
        bool validBefore = isValid();
        int oldIndex = index;
        index = batteryIndex;
        bool validNow = isValid();
        if (validBefore != validNow)
            Q_EMIT validChanged(validNow);

        if (validNow) {
            if (validBefore) {
                // valid now, valid before so we have to check individual values

                // ignore chargerType - it won't change based on battery index
                //Q_EMIT chargerTypeChanged(newChargerType);

                LomiriBatteryInfo::ChargingState newChargingState = chargingState();
                if (newChargingState != chargingState(oldIndex))
                    Q_EMIT chargingStateChanged(newChargingState);

                int newValue = level();
                if (newValue != level(oldIndex))
                    Q_EMIT levelChanged(newValue);

                newValue = currentFlow();
                if (newValue != currentFlow(oldIndex))
                    Q_EMIT currentFlowChanged(newValue);

                newValue = cycleCount();
                if (newValue != cycleCount(oldIndex))
                    Q_EMIT cycleCountChanged(newValue);

                newValue = remainingCapacity();
                if (newValue != remainingCapacity(oldIndex))
                    Q_EMIT remainingCapacityChanged(newValue);

                newValue = remainingChargingTime();
                if (newValue != remainingChargingTime(oldIndex))
                    Q_EMIT remainingChargingTimeChanged(newValue);

                newValue = voltage();
                if (newValue != voltage(oldIndex))
                    Q_EMIT voltageChanged(newValue);

                LomiriBatteryInfo::LevelStatus newLevelStatus = levelStatus();
                if (newLevelStatus != levelStatus(oldIndex))
                    Q_EMIT levelStatusChanged(newLevelStatus);

                LomiriBatteryInfo::Health newHealth = health();
                if (newHealth != health(oldIndex))
                    Q_EMIT healthChanged(newHealth);

                float newTemperature = temperature();
                if (!qFuzzyCompare(newTemperature, temperature(oldIndex)))
                    Q_EMIT temperatureChanged(newTemperature);
            } else {
                // it wasn't valid before so everything is changed

                // ignore chargerType - it won't change based on battery index
                //Q_EMIT chargerTypeChanged(newChargerType);

                Q_EMIT chargingStateChanged(chargingState());
                Q_EMIT levelChanged(level());
                Q_EMIT currentFlowChanged(currentFlow());
                Q_EMIT cycleCountChanged(cycleCount());
                Q_EMIT remainingCapacityChanged(remainingCapacity());
                Q_EMIT remainingChargingTimeChanged(remainingChargingTime());
                Q_EMIT voltageChanged(voltage());
                Q_EMIT levelStatusChanged(levelStatus());
                Q_EMIT healthChanged(health());
                Q_EMIT temperatureChanged(temperature());
            }
        }

        Q_EMIT batteryIndexChanged(index);
    }
}

int LomiriBatteryInfoPrivate::level(int battery)
{
    int maxCapacity = maximumCapacity(battery);
    int remCapacity = remainingCapacity(battery);

    if (maxCapacity == 0)
        return -1;

    return remCapacity * 100 / maxCapacity;
}

int LomiriBatteryInfoPrivate::level()
{
    return level(index);
}

int LomiriBatteryInfoPrivate::currentFlow(int battery)
{
    if (!watchCurrentFlow)
        return getCurrentFlow(battery);

    return currentFlows.value(battery);
}

int LomiriBatteryInfoPrivate::currentFlow()
{
    return currentFlow(index);
}

int LomiriBatteryInfoPrivate::cycleCount(int battery)
{
    Q_UNUSED(battery)

    return -1;
}

int LomiriBatteryInfoPrivate::cycleCount()
{
    return cycleCount(index);
}

int LomiriBatteryInfoPrivate::maximumCapacity(int battery)
{
    if (maximumCapacities[battery] == 0) {
        QFile maximum(BATTERY_SYSFS_PATH()->arg(battery) + QStringLiteral("charge_full"));
        if (maximum.open(QIODevice::ReadOnly)) {
            bool ok = false;
            int capacity = maximum.readAll().simplified().toInt(&ok);
            if (ok)
                maximumCapacities[battery] = capacity / 1000;
            else
                maximumCapacities[battery] = -1;
        } else {
            maximumCapacities[battery] = -1;
        }
    }

    return maximumCapacities[battery];
}

int LomiriBatteryInfoPrivate::maximumCapacity()
{
    return maximumCapacity(index);
}

int LomiriBatteryInfoPrivate::remainingCapacity(int battery)
{
    if (!watchRemainingCapacity)
        return getRemainingCapacity(battery);

    return remainingCapacities.value(battery);
}

int LomiriBatteryInfoPrivate::remainingCapacity()
{
    return remainingCapacity(index);
}

int LomiriBatteryInfoPrivate::remainingChargingTime(int battery)
{
    if (!watchRemainingChargingTime)
        return getRemainingChargingTime(battery);

    return remainingChargingTimes.value(battery);
}

int LomiriBatteryInfoPrivate::remainingChargingTime()
{
    return remainingChargingTime(index);
}

int LomiriBatteryInfoPrivate::voltage(int battery)
{
    if (!watchVoltage)
        return getVoltage(battery);

    return voltages.value(battery);
}

int LomiriBatteryInfoPrivate::voltage()
{
    return voltage(index);
}

LomiriBatteryInfo::ChargerType LomiriBatteryInfoPrivate::chargerType()
{
    if (!watchChargerType)
        return getChargerType();

    return currentChargerType;
}

LomiriBatteryInfo::ChargingState LomiriBatteryInfoPrivate::chargingState(int battery)
{
    if (!watchChargingState)
        return getChargingState(battery);

    return chargingStates.value(battery);
}

LomiriBatteryInfo::ChargingState LomiriBatteryInfoPrivate::chargingState()
{
    return chargingState(index);
}

LomiriBatteryInfo::LevelStatus LomiriBatteryInfoPrivate::levelStatus(int battery)
{
    if (!watchLevelStatus)
        return getLevelStatus(battery);

    return levelStatuss.value(battery);
}

LomiriBatteryInfo::LevelStatus LomiriBatteryInfoPrivate::levelStatus()
{
    return levelStatus(index);
}

LomiriBatteryInfo::Health LomiriBatteryInfoPrivate::health(int battery)
{
    Q_UNUSED(battery)

    return LomiriBatteryInfo::HealthUnknown;
}

LomiriBatteryInfo::Health LomiriBatteryInfoPrivate::health()
{
    return health(index);
}

float LomiriBatteryInfoPrivate::temperature(int battery)
{
    Q_UNUSED(battery)

    return qQNaN();
}

float LomiriBatteryInfoPrivate::temperature()
{
    return temperature(index);
}

void LomiriBatteryInfoPrivate::connectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod batteryCountChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::batteryCountChanged);
    static const QMetaMethod validChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::validChanged);
    static const QMetaMethod chargerTypeChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::chargerTypeChanged);
    static const QMetaMethod chargingStateChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::chargingStateChanged);
    static const QMetaMethod currentFlowChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::currentFlowChanged);
    static const QMetaMethod remainingCapacityChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::remainingCapacityChanged);
    static const QMetaMethod remainingChargingTimeChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::remainingChargingTimeChanged);
    static const QMetaMethod voltageChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::voltageChanged);
    static const QMetaMethod levelStatusChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::levelStatusChanged);

#if !defined(QT_NO_UDEV)
    if (!uDevWrapper)
        uDevWrapper = new QUDevWrapper(this);
    if (!watchChargerType && signal == chargerTypeChangedSignal) {
        connect(uDevWrapper, SIGNAL(chargerTypeChanged(QByteArray,bool)), this, SLOT(onChargerTypeChanged(QByteArray,bool)));
    } else if (!watchIsValid && !watchCurrentFlow && !watchVoltage && !watchChargingState && !watchRemainingCapacity
               && !watchRemainingChargingTime && !watchBatteryCount && !watchLevelStatus) {
        connect(uDevWrapper, SIGNAL(batteryDataChanged(int,QByteArray,QByteArray)), this, SLOT(onBatteryDataChanged(int,QByteArray,QByteArray)));
    }
#else
    if (timer == 0) {
       timer = new QTimer;
       timer->setInterval(2000);
       connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    }

    if (!timer->isActive())
       timer->start();
#endif // QT_NO_UDEV

    if (signal == validChangedSignal) {
        if (!watchIsValid && !watchBatteryCount)
            forceWatchBatteryCount = true;

        watchIsValid = true;

        // we have to watch battery count if someone is watching validChanged.
        watchBatteryCount = true;
        batteryCounts = getBatteryCount();
    } else if (signal == batteryCountChangedSignal) {
        watchBatteryCount = true;
        forceWatchBatteryCount = false;
        batteryCounts = getBatteryCount();
    } else if (signal == currentFlowChangedSignal) {
        watchCurrentFlow = true;
        int count = batteryCount();
        for (int i = 0; i < count; ++i)
            currentFlows[i] = getCurrentFlow(i);
    } else if (signal == voltageChangedSignal) {
        watchVoltage = true;
        int count = batteryCount();
        for (int i = 0; i < count; ++i)
            voltages[i] = getVoltage(i);
    } else if (signal == remainingCapacityChangedSignal) {
        watchRemainingCapacity = true;
        int count = batteryCount();
        for (int i = 0; i < count; ++i)
            remainingCapacities[i] = getRemainingCapacity(i);
    } else if (signal == remainingChargingTimeChangedSignal) {
        watchRemainingChargingTime = true;
        int count = batteryCount();
        for (int i = 0; i < count; ++i)
            remainingChargingTimes[i] = getRemainingChargingTime(i);
    } else if (signal == chargerTypeChangedSignal) {
        watchChargerType = true;
        currentChargerType = getChargerType();
    } else if (signal == chargingStateChangedSignal) {
        watchChargingState = true;
        int count = batteryCount();
        for (int i = 0; i < count; ++i)
            chargingStates[i] = getChargingState(i);
    } else if (signal == levelStatusChangedSignal) {
        watchLevelStatus = true;
        int count = batteryCount();
        for (int i = 0; i < count; i++)
            levelStatuss[i] = getLevelStatus(i);
    }
}

void LomiriBatteryInfoPrivate::disconnectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod batteryCountChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::batteryCountChanged);
    static const QMetaMethod validChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::validChanged);
    static const QMetaMethod chargerTypeChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::chargerTypeChanged);
    static const QMetaMethod chargingStateChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::chargingStateChanged);
    static const QMetaMethod currentFlowChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::currentFlowChanged);
    static const QMetaMethod remainingCapacityChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::remainingCapacityChanged);
    static const QMetaMethod remainingChargingTimeChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::remainingChargingTimeChanged);
    static const QMetaMethod voltageChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::voltageChanged);
    static const QMetaMethod levelStatusChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoPrivate::levelStatusChanged);

    if (signal == validChangedSignal) {
        watchIsValid = false;
        if (forceWatchBatteryCount) {
            watchBatteryCount = false;
            batteryCounts = -1;
        }
    } else if (signal == batteryCountChangedSignal) {
        if (!watchIsValid) {
            watchBatteryCount = false;
            batteryCounts = -1;
        } else {
            forceWatchBatteryCount = true;
        }
    } else if (signal == currentFlowChangedSignal) {
        watchCurrentFlow = false;
        currentFlows.clear();
    } else if (signal == voltageChangedSignal) {
        watchVoltage = false;
        voltages.clear();
    } else if (signal == remainingCapacityChangedSignal) {
        watchRemainingCapacity = false;
        remainingCapacities.clear();
    } else if (signal == remainingChargingTimeChangedSignal) {
        watchRemainingChargingTime = false;
        remainingChargingTimes.clear();
    } else if (signal == chargerTypeChangedSignal) {
        watchChargerType = false;
        currentChargerType = LomiriBatteryInfo::UnknownCharger;
    } else if (signal == chargingStateChangedSignal) {
        watchChargingState = false;
        chargingStates.clear();
    } else if (signal == levelStatusChangedSignal) {
        watchLevelStatus = false;
        levelStatuss.clear();
    }

#if !defined(QT_NO_UDEV)
    if (uDevWrapper && !watchChargerType && signal == chargerTypeChangedSignal) {
        disconnect(uDevWrapper, SIGNAL(chargerTypeChanged(QByteArray,bool)),
                   this, SLOT(onChargerTypeChanged(QByteArray,bool)));
    } else if (uDevWrapper && !watchCurrentFlow && !watchVoltage && !watchChargingState && !watchRemainingCapacity
               && !watchRemainingChargingTime && !watchBatteryCount && !watchLevelStatus) {
        disconnect(uDevWrapper, SIGNAL(batteryDataChanged(int,QByteArray,QByteArray)),
                   this, SLOT(onBatteryDataChanged(int,QByteArray,QByteArray)));
    }
#endif

    if (!watchBatteryCount && !watchChargerType && !watchChargingState
            && !watchCurrentFlow && !watchRemainingCapacity
            && !watchRemainingChargingTime && !watchVoltage && !watchLevelStatus) {
#if !defined(QT_NO_UDEV)
        if (uDevWrapper) {
            delete uDevWrapper;
            uDevWrapper = 0;
        }
#else
        timer->stop();
#endif // QT_NO_UDEV
    }
}

#if !defined(QT_NO_UDEV)

void LomiriBatteryInfoPrivate::onBatteryDataChanged(int battery, const QByteArray &attribute, const QByteArray &value)
{
    if (watchBatteryCount) {
        int count = getBatteryCount();
        if (batteryCounts != count) {
            bool validBefore = isValid();
            batteryCounts = count;
            bool validNow = isValid();
            if (validBefore != validNow)
                Q_EMIT validChanged(validNow);

            // We do not have to worry about firing all changed signals here.
            // Each individual value will receive an onBatteryDataChanged() call
            // and will fire a signal at that time.

            Q_EMIT batteryCountChanged(count);
        }
    }

    if (watchChargingState && attribute.contains("status")) {
        LomiriBatteryInfo::ChargingState state = LomiriBatteryInfo::UnknownChargingState;
        if (qstrcmp(value, "Charging") == 0)
            state = LomiriBatteryInfo::Charging;
        else if (qstrcmp(value, "Not charging") == 0)
            state = LomiriBatteryInfo::IdleChargingState;
        else if (qstrcmp(value, "Discharging") == 0)
            state = LomiriBatteryInfo::Discharging;
        else if (qstrcmp(value, "Full") == 0)
            state = LomiriBatteryInfo::IdleChargingState;
        if (chargingStates.value(battery) != state) {
            chargingStates[battery] = state;
            if (battery == index)
                Q_EMIT chargingStateChanged(state);
        }
    }

    if (watchRemainingCapacity && attribute.contains("charge_now")) {
        if (!value.isEmpty()) {
            int remainingCapacity = value.toInt() / 1000;
            if (remainingCapacities.value(battery) != remainingCapacity) {
                remainingCapacities[battery] = remainingCapacity;
                if (battery == index)
                    Q_EMIT remainingCapacityChanged(remainingCapacity);
            }
        }
    }

    if (watchRemainingChargingTime && attribute.contains("time_to_full_avg")) {
        if (!value.isEmpty()) {
            int remainingChargingTime = value.toInt();
            if (remainingChargingTimes.value(battery) != remainingChargingTime) {
                remainingChargingTimes[battery] = remainingChargingTime;
                if (battery == index)
                    Q_EMIT remainingChargingTimeChanged(remainingChargingTime);
            }
        }
    }

    if (watchVoltage && attribute.contains("voltage_now")) {
        if (!value.isEmpty()) {
            int voltage = value.toInt() / 1000;
            if (voltages.value(battery) != voltage) {
                voltages[battery] = voltage;
                if (battery == index)
                    Q_EMIT voltageChanged(voltage);
            }
        }
    }

    if (watchCurrentFlow && attribute.contains("current_now")) {
        if (!value.isEmpty()) {
            int currentFlow = value.toInt() / -1000;
            if (chargingStates.value(battery) == LomiriBatteryInfo::Discharging && currentFlow < 0)
                currentFlow = -currentFlow;

            if (currentFlows.value(battery) != currentFlow) {
                currentFlows[battery] = currentFlow;
                if (battery == index)
                    Q_EMIT currentFlowChanged(currentFlow);
            }
        }
    }

    if (watchLevelStatus && attribute.contains("capacity_level")) {
        LomiriBatteryInfo::LevelStatus levelStatus = LomiriBatteryInfo::LevelUnknown;
        if (qstrcmp(value, "Critical") == 0)
            levelStatus = LomiriBatteryInfo::LevelEmpty;
        else if (qstrcmp(value, "Low") == 0)
            levelStatus = LomiriBatteryInfo::LevelLow;
        else if (qstrcmp(value, "Normal") == 0)
            levelStatus = LomiriBatteryInfo::LevelOk;
        else if (qstrcmp(value, "Full") == 0)
            levelStatus = LomiriBatteryInfo::LevelFull;
        if (levelStatuss.value(battery) != levelStatus) {
            levelStatuss[battery] = levelStatus;
            if (battery == index)
                Q_EMIT levelStatusChanged(levelStatus);
        }
    }
}

void LomiriBatteryInfoPrivate::onChargerTypeChanged(const QByteArray &value, bool enabled)
{
    if (watchChargerType) {
        LomiriBatteryInfo::ChargerType charger = LomiriBatteryInfo::UnknownCharger;
        if (enabled) {
            if ((qstrcmp(value, "AC") == 0) || qstrcmp(value, "USB_DCP") == 0)
                charger = LomiriBatteryInfo::WallCharger;
            else if (qstrcmp(value, "USB") == 0)
                charger = LomiriBatteryInfo::USBCharger;
            else if (qstrcmp(value, "USB_CDP") == 0 || qstrcmp(value, "USB_SDP") == 0)
                charger = LomiriBatteryInfo::VariableCurrentCharger;
        }
        if (currentChargerType != charger) {
            currentChargerType = charger;
            Q_EMIT chargerTypeChanged(charger);
        }
    }
}

#else

void LomiriBatteryInfoPrivate::onTimeout()
{
    int count = getBatteryCount();
    int value;
    if (watchBatteryCount) {
        value = getBatteryCount();
        if (batteryCounts != value) {
            bool validBefore = isValid();
            batteryCounts = value;
            bool validNow = isValid();
            if (validBefore != validNow)
                Q_EMIT validChanged(validNow);

            // We do not have to worry about firing all changed signals here.
            // Each individual value will (possibly) be updated below
            // and will fire a signal at that time if it has changed.

            Q_EMIT batteryCountChanged(value);
        }
    }

    for (int i = 0; i < count; ++i) {
        if (watchCurrentFlow) {
            value = getCurrentFlow(i);
            if (currentFlows.value(i) != value) {
                currentFlows[i] = value;
                if (i == index)
                    Q_EMIT currentFlowChanged(value);
            }
        }

        if (watchVoltage) {
            value = getVoltage(i);
            if (voltages.value(i) != value) {
                voltages[i] = value;
                if (i == index)
                    Q_EMIT voltageChanged(value);
            }
        }

        if (watchRemainingCapacity) {
            value = getRemainingCapacity(i);
            if (remainingCapacities.value(i) != value) {
                remainingCapacities[i] = value;
                if (i == index)
                    Q_EMIT remainingCapacityChanged(value);
            }
        }

        if (watchRemainingChargingTime) {
            value = getRemainingChargingTime(i);
            if (remainingChargingTimes.value(i) != value) {
                remainingChargingTimes[i] = value;
                if (i == index)
                    Q_EMIT remainingChargingTimeChanged(value);
            }
        }

        if (watchChargerType) {
            LomiriBatteryInfo::ChargerType charger = getChargerType();
            if (currentChargerType != charger) {
                currentChargerType = charger;
                Q_EMIT chargerTypeChanged(charger);
            }
        }

        if (watchChargingState) {
            LomiriBatteryInfo::ChargingState state = getChargingState(i);
            if (chargingStates.value(i) != state) {
                chargingStates[i] = state;
                if (i == index)
                    Q_EMIT chargingStateChanged(state);
            }
        }

        if (watchLevelStatus) {
            LomiriBatteryInfo::LevelStatus levelStatus = getLevelStatus(i);
            if (levelStatuss.value(i) != levelStatus) {
                levelStatuss[i] = levelStatus;
                if (i == index)
                    Q_EMIT levelStatusChanged(levelStatus);
            }
        }
    }
}

#endif // QT_NO_UDEV

int LomiriBatteryInfoPrivate::getBatteryCount()
{
    return QDir(*POWER_SUPPLY_SYSFS_PATH()).entryList(QStringList() << QStringLiteral("BAT*")).size();
}

int LomiriBatteryInfoPrivate::getCurrentFlow(int battery)
{
    LomiriBatteryInfo::ChargingState state = chargingState(battery);
    if (state == LomiriBatteryInfo::UnknownChargingState)
        return 0;

    QFile current(BATTERY_SYSFS_PATH()->arg(battery) + QStringLiteral("current_now"));
    if (!current.open(QIODevice::ReadOnly))
        return 0;

    bool ok = false;
    int flow = current.readAll().simplified().toInt(&ok);
    if (ok) {
        // We want discharging current to be positive and charging current to be negative.
        if (state == LomiriBatteryInfo::Charging) {
          // In case some drivers make charging current negative already and others are opposite
          return flow < 0 ? flow / 1000 : flow / -1000;
        } else if (state == LomiriBatteryInfo::Discharging) {
          // In case some drivers make discharging current positive already and others are opposite
          return flow > 0 ? flow / 1000 : flow / -1000;
        }
    }

    return 0;
}

int LomiriBatteryInfoPrivate::getRemainingCapacity(int battery)
{
    QFile remaining(BATTERY_SYSFS_PATH()->arg(battery) + QStringLiteral("charge_now"));
    if (!remaining.open(QIODevice::ReadOnly))
        return -1;

    bool ok = false;
    int capacity = remaining.readAll().simplified().toInt(&ok);
    if (ok)
        return capacity / 1000;
    return -1;
}

int LomiriBatteryInfoPrivate::getRemainingChargingTime(int battery)
{
    LomiriBatteryInfo::ChargingState state = chargingState(battery);
    if (state == LomiriBatteryInfo::UnknownChargingState)
        return -1;
    else if (state == LomiriBatteryInfo::IdleChargingState || state == LomiriBatteryInfo::Discharging)
        return 0;

    int remaining = 0;
    QFile timeToFull(BATTERY_SYSFS_PATH()->arg(battery) + QStringLiteral("time_to_full_avg"));
    if (timeToFull.open(QIODevice::ReadOnly)) {
        bool ok = false;
        remaining = timeToFull.readAll().simplified().toInt(&ok);
        if (ok)
            return remaining;
        return -1;
    }

    int max = 0;
    int current = 0;
    if ((max = maximumCapacity(battery)) == -1
        || (remaining = remainingCapacity(battery)) == -1
        || (current = currentFlow(battery)) == 0) {
        return -1;
    }
    return (max - remaining) * -3600 / current;
}

int LomiriBatteryInfoPrivate::getVoltage(int battery)
{
    QFile current(BATTERY_SYSFS_PATH()->arg(battery) + QStringLiteral("voltage_now"));
    if (!current.open(QIODevice::ReadOnly))
        return -1;

    bool ok = false;
    int voltage = current.readAll().simplified().toInt(&ok);
    if (ok)
        return voltage / 1000;
    return -1;
}

LomiriBatteryInfo::ChargerType LomiriBatteryInfoPrivate::getChargerType()
{
    QFile charger(*AC_ONLINE_SYSFS_PATH());
    if (charger.open(QIODevice::ReadOnly)) {
        char online;
        if (charger.read(&online, 1) == 1 && online == '1')
            return LomiriBatteryInfo::WallCharger;
        charger.close();
    }

    QMap<QString, QString> chargerMap;
    chargerMap.insert(*USB0_PRESENT_SYSFS_PATH(), *USB0_TYPE_SYSFS_PATH());
    chargerMap.insert(*USB_PRESENT_SYSFS_PATH(), *USB_TYPE_SYSFS_PATH());

    QList<QString> presentPaths = chargerMap.keys();
    Q_FOREACH (const QString &presentPath, presentPaths) {
        charger.setFileName(presentPath);
        if (charger.open(QIODevice::ReadOnly)) {
            char present;
            if (charger.read(&present, 1) == 1 && present == '1') {
                charger.close();

                charger.setFileName(chargerMap.value(presentPath));
                if (charger.open(QIODevice::ReadOnly)) {
                    if (charger.readAll().simplified() == "USB_DCP")
                        return LomiriBatteryInfo::WallCharger;
                    return LomiriBatteryInfo::USBCharger;
                }
            }
            charger.close();
        }
    }

    return LomiriBatteryInfo::UnknownCharger;
}

LomiriBatteryInfo::ChargingState LomiriBatteryInfoPrivate::getChargingState(int battery)
{
    QFile state(BATTERY_SYSFS_PATH()->arg(battery) + QStringLiteral("status"));
    if (!state.open(QIODevice::ReadOnly))
        return LomiriBatteryInfo::UnknownChargingState;

    QByteArray status = state.readAll().simplified();
    if (status == "Charging")
        return LomiriBatteryInfo::Charging;
    else if (status == "Not charging")
        return LomiriBatteryInfo::IdleChargingState;
    else if (status == "Discharging")
        return LomiriBatteryInfo::Discharging;
    else if (status == "Full")
        return LomiriBatteryInfo::IdleChargingState;

    return LomiriBatteryInfo::UnknownChargingState;
}

LomiriBatteryInfo::LevelStatus LomiriBatteryInfoPrivate::getLevelStatus(int battery)
{
    QFile levelStatusFile(BATTERY_SYSFS_PATH()->arg(battery) + QStringLiteral("capacity_level"));
    if (!levelStatusFile.open(QIODevice::ReadOnly))
        return LomiriBatteryInfo::LevelUnknown;

    QByteArray levelStatus = levelStatusFile.readAll().simplified();
    if (qstrcmp(levelStatus, "Critical") == 0)
        return LomiriBatteryInfo::LevelEmpty;
    else if (qstrcmp(levelStatus, "Low") == 0)
        return LomiriBatteryInfo::LevelLow;
    else if (qstrcmp(levelStatus, "Normal") == 0)
        return LomiriBatteryInfo::LevelOk;
    else if (qstrcmp(levelStatus, "Full") == 0)
        return LomiriBatteryInfo::LevelFull;

    return LomiriBatteryInfo::LevelUnknown;
}

QT_END_NAMESPACE
