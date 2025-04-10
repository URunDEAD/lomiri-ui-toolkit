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

#include "lomiribatteryinfo_upower_p.h"

#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qtimer.h>
#include <QtCore/qnumeric.h>
#include <QFile>
#include "lomiridevicekitservice_linux_p.h"


QT_BEGIN_NAMESPACE

LomiriBatteryInfoPrivate::LomiriBatteryInfoPrivate(LomiriBatteryInfo *parent)
    : QObject(parent),
      cType(LomiriBatteryInfo::UnknownCharger),
      cState(LomiriBatteryInfo::UnknownChargingState),
      q_ptr(parent),
      index(0)
{
    initialize();
}

LomiriBatteryInfoPrivate::LomiriBatteryInfoPrivate(int batteryIndex, LomiriBatteryInfo *parent)
    : QObject(parent),
      cType(LomiriBatteryInfo::UnknownCharger),
      cState(LomiriBatteryInfo::UnknownChargingState),
      q_ptr(parent),
      index(batteryIndex)
{
    initialize();
}

void LomiriBatteryInfoPrivate::initialize()
{
    watcher = new QDBusServiceWatcher(QStringLiteral("org.freedesktop.UPower"),QDBusConnection::systemBus(),
                                      QDBusServiceWatcher::WatchForRegistration |
                                      QDBusServiceWatcher::WatchForUnregistration, this);
    connect(watcher, SIGNAL(serviceRegistered(QString)),
            this, SLOT(connectToUpower()));
    connect(watcher, SIGNAL(serviceUnregistered(QString)),
            this, SLOT(disconnectFromUpower()));

    bool uPowerAvailable = QDBusConnection::systemBus().interface()->isServiceRegistered(QStringLiteral("org.freedesktop.UPower"));

    if (uPowerAvailable)
        connectToUpower();
}

LomiriBatteryInfoPrivate::~LomiriBatteryInfoPrivate()
{
}

void LomiriBatteryInfoPrivate::connectToUpower()
{
    getBatteryStats();
}

void LomiriBatteryInfoPrivate::disconnectFromUpower()
{
}

int LomiriBatteryInfoPrivate::batteryCount()
{
    return batteryMap.count();
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
    if (batteryMap.count() >= battery)
        return (batteryMap.value(battery).value(QStringLiteral("EnergyRate")).toDouble()
                / (batteryMap.value(battery).value(QStringLiteral("Voltage")).toDouble()) * 1000);
    else
        return 0;
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
    if (batteryMap.count() >= battery)
        return batteryMap.value(battery).value(QStringLiteral("EnergyFull")).toDouble() * 1000;
    else
        return 0;
}

int LomiriBatteryInfoPrivate::maximumCapacity()
{
    return maximumCapacity(index);
}

int LomiriBatteryInfoPrivate::remainingCapacity(int battery)
{
    if (batteryMap.count() >= battery)
        return batteryMap.value(battery).value(QStringLiteral("Energy")).toDouble() * 1000;
    else
        return 0;
}

int LomiriBatteryInfoPrivate::remainingCapacity()
{
    return remainingCapacity(index);
}

int LomiriBatteryInfoPrivate::remainingChargingTime(int battery)
{
    if (batteryMap.count() >= battery)
        return batteryMap.value(battery).value(QStringLiteral("TimeToFull")).toInt();
    else
        return 0;
}

int LomiriBatteryInfoPrivate::remainingChargingTime()
{
    return remainingChargingTime(index);
}

int LomiriBatteryInfoPrivate::voltage(int battery)
{
    if (batteryMap.count() >= battery)
        return (batteryMap.value(battery).value(QStringLiteral("Voltage")).toDouble() * 1000);
    else
        return 0;
}

int LomiriBatteryInfoPrivate::voltage()
{
    return voltage(index);
}

LomiriBatteryInfo::ChargerType LomiriBatteryInfoPrivate::chargerType()
{
    return cType;
}

LomiriBatteryInfo::ChargingState LomiriBatteryInfoPrivate::chargingState(int battery)
{
    Q_UNUSED(battery)
    return cState;
}

LomiriBatteryInfo::ChargingState LomiriBatteryInfoPrivate::chargingState()
{
    return chargingState(index);
}

LomiriBatteryInfo::LevelStatus LomiriBatteryInfoPrivate::levelStatus(int battery)
{
    LomiriBatteryInfo::LevelStatus stat = LomiriBatteryInfo::LevelUnknown;

    if (batteryMap.count() >= battery) {
        int level = batteryMap.value(battery).value(QStringLiteral("Percentage")).toInt();
        if (level < 3)
            stat = LomiriBatteryInfo::LevelEmpty;
        else if (level < 11)
            stat = LomiriBatteryInfo::LevelLow;
        else if (level < 99)
            stat = LomiriBatteryInfo::LevelOk;
        else
            stat = LomiriBatteryInfo::LevelFull;
    }
    return stat;
}

LomiriBatteryInfo::LevelStatus LomiriBatteryInfoPrivate::levelStatus()
{
    return levelStatus(index);
}

LomiriBatteryInfo::Health LomiriBatteryInfoPrivate::health(int battery)
{
    LomiriBatteryInfo::Health health = LomiriBatteryInfo::HealthUnknown;
    if (batteryMap.count() >= battery) {
        int percent = (batteryMap.value(battery).value(QStringLiteral("EnergyFull")).toInt() *100)
                / (float)( batteryMap.value(battery).value(QStringLiteral("EnergyFullDesign")).toInt());
        if (percent < 65)
            health = LomiriBatteryInfo::HealthBad;
        else
            health = LomiriBatteryInfo::HealthOk;
    }
    return health;
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

void LomiriBatteryInfoPrivate::uPowerBatteryPropertyChanged(const QString &prop, const QVariant &v)
{
    QUPowerDeviceInterface *uPowerDevice = qobject_cast<QUPowerDeviceInterface*>(sender());
    int foundBattery = 0;

    if (uPowerDevice->type() == 2) {
        QMapIterator<int, QVariantMap> i(batteryMap);
        while (i.hasNext()) {
            i.next();
            if (i.value().value(QStringLiteral("NativePath")).toString() == uPowerDevice->nativePath()) {
                foundBattery = i.key();
                break;
            }
        }

        QVariantMap foundMap = batteryMap.value(foundBattery);
        foundMap.insert(prop,v);
        batteryMap.insert(foundBattery,foundMap);
    }

    if (prop == QLatin1String("Energy")) {
        if (foundBattery == index)
            Q_EMIT remainingCapacityChanged(v.toDouble() * 1000);

    } else if (prop == QLatin1String("EnergyRate")) {
        if (foundBattery == index)
            Q_EMIT currentFlowChanged(v.toDouble() / (uPowerDevice->voltage() * 1000));

    } else if (prop == QLatin1String("Percentage")) {
        int level = v.toInt();

        LomiriBatteryInfo::LevelStatus stat = LomiriBatteryInfo::LevelUnknown;

        if (level < 3)
            stat = LomiriBatteryInfo::LevelEmpty;
        else if (level < 11)
            stat = LomiriBatteryInfo::LevelLow;
        else if (level < 99)
            stat = LomiriBatteryInfo::LevelOk;
        else
            stat = LomiriBatteryInfo::LevelFull;

        if (foundBattery == index)
            Q_EMIT levelStatusChanged(stat);

    } else if (prop == QLatin1String("Voltage")) {
        if (foundBattery == index)
            Q_EMIT voltageChanged(v.toDouble() * 1000 );

    } else if (prop == QLatin1String("State")) {

        LomiriBatteryInfo::ChargingState curChargeState = getCurrentChargingState(v.toInt());

        if (curChargeState != cState) {
            cState = curChargeState;
            if (foundBattery == index)
                Q_EMIT chargingStateChanged(curChargeState);
        }
    } else if (prop == QLatin1String("Capacity")) {
        LomiriBatteryInfo::Health newHealth = health(index);
        if (newHealth != healthList.value(index)) {
            healthList.insert(index,newHealth);
            Q_EMIT healthChanged(newHealth);
        }

    } else if (prop == QLatin1String("TimeToFull")) {
        if (foundBattery == index)
            Q_EMIT remainingChargingTimeChanged(v.toInt());
    } else if (prop == QLatin1String("Online")) {
        LomiriBatteryInfo::ChargerType curCharger = LomiriBatteryInfo::UnknownCharger;
        if (v.toBool()) {
            curCharger = getChargerType(uPowerDevice->nativePath());
        }
        if (curCharger != cType) {
            cType = curCharger;
            Q_EMIT chargerTypeChanged(cType);
        }
    }
}

LomiriBatteryInfo::ChargerType LomiriBatteryInfoPrivate::getChargerType(const QString &path)
{
    LomiriBatteryInfo::ChargerType chargerType = LomiriBatteryInfo::UnknownCharger;
    QFile charger;
    charger.setFileName(QStringLiteral("/sys/class/power_supply/") + path + QStringLiteral("/type"));
    if (charger.open(QIODevice::ReadOnly)) {
        QString line = QString::fromLocal8Bit(charger.readAll().simplified());
        if (line  == QStringLiteral("USB")) {
            chargerType = LomiriBatteryInfo::USBCharger;
        } else if (line == QStringLiteral("Mains")) {
            chargerType = LomiriBatteryInfo::WallCharger;
        }
    }
    charger.close();
    return chargerType;
}

LomiriBatteryInfo::ChargingState LomiriBatteryInfoPrivate::getCurrentChargingState(int state)
{
    LomiriBatteryInfo::ChargingState curChargeState = LomiriBatteryInfo::UnknownChargingState;
    switch (state) {
    case 1: // charging
    {
        curChargeState = LomiriBatteryInfo::Charging;
    }
        break;
    case 2: //discharging
    case 3: //empty
        curChargeState = LomiriBatteryInfo::Discharging;
        break;
    case 4: //fully charged
        curChargeState = LomiriBatteryInfo::IdleChargingState;
        break;
    case 5: //pending charge
    case 6: //pending discharge
        break;
    default:
        curChargeState = LomiriBatteryInfo::UnknownChargingState;
        break;
    };

    return curChargeState;
}

void LomiriBatteryInfoPrivate::getBatteryStats()
{
    batteryMap.clear();
    QUPowerInterface *power;
    power = new QUPowerInterface(this);

    connect(power,SIGNAL(deviceAdded(QString)),
            this,SLOT(deviceAdded(QString)));
    connect(power,SIGNAL(deviceRemoved(QString)),
            this,SLOT(deviceRemoved(QString)));

    Q_FOREACH (const QDBusObjectPath &objpath, power->enumerateDevices()) {
        QUPowerDeviceInterface *uPowerDevice;
        uPowerDevice = new QUPowerDeviceInterface(objpath.path(),this);
        connect(uPowerDevice,SIGNAL(propertyChanged(QString,QVariant)),
                this,SLOT(uPowerBatteryPropertyChanged(QString,QVariant)));


        QMapIterator<QString, QVariant> i(uPowerDevice->getProperties());
        while (i.hasNext()) {
            i.next();
            QString prop = i.key();
            QVariant v = i.value();

            int foundBattery = 0;
            if (uPowerDevice->type() == 2) {

                QMapIterator<int, QVariantMap> i(batteryMap);
                while (i.hasNext()) {
                    i.next();
                    if (i.value().value(QStringLiteral("NativePath")).toString() == uPowerDevice->nativePath()) {
                        foundBattery = i.key();
                        break;
                    }
                }

                QVariantMap foundMap = batteryMap.value(foundBattery);
                foundMap.insert(prop,v);
                batteryMap.insert(foundBattery,foundMap);
            }

            if (prop == QLatin1String("Energy")) {
                if (foundBattery == index)
                    Q_EMIT remainingCapacityChanged(v.toDouble() * 1000);
            } else if (prop == QLatin1String("EnergyFullDesign")) {

            } else if (prop == QLatin1String("EnergyRate")) {
                if (foundBattery == index)
                    Q_EMIT currentFlowChanged(v.toDouble() / (uPowerDevice->voltage() * 1000));

            } else if (prop == QLatin1String("Percentage")) {
                int level = v.toInt();

                LomiriBatteryInfo::LevelStatus stat = LomiriBatteryInfo::LevelUnknown;

                if (level < 3)
                    stat = LomiriBatteryInfo::LevelEmpty;
                else if (level < 11)
                    stat = LomiriBatteryInfo::LevelLow;
                else if (level < 99)
                    stat = LomiriBatteryInfo::LevelOk;
                else
                    stat = LomiriBatteryInfo::LevelFull;

                if (foundBattery == index)
                    Q_EMIT levelStatusChanged(stat);

            } else if (prop == QLatin1String("Voltage")) {
                if (foundBattery == index)
                    Q_EMIT voltageChanged(v.toDouble() * 1000 );

            } else if (prop == QLatin1String("State")) {

                LomiriBatteryInfo::ChargingState curChargeState = getCurrentChargingState(v.toInt());

                if (curChargeState != cState) {
                    cState = curChargeState;
                    if (foundBattery == index)
                        Q_EMIT chargingStateChanged(curChargeState);
                }

            } else if (prop == QLatin1String("Capacity")) {

                LomiriBatteryInfo::Health newHealth = health(index);
                healthList.insert(index,newHealth);
                Q_EMIT healthChanged(newHealth);

            } else if (prop == QLatin1String("TimeToFull")) {
                Q_EMIT remainingChargingTimeChanged(v.toInt());
            } else if (prop == QLatin1String("Online")) {
                LomiriBatteryInfo::ChargerType curCharger = LomiriBatteryInfo::UnknownCharger;
                if (v.toBool()) {
                    curCharger = getChargerType(uPowerDevice->nativePath());
                    if (curCharger != cType) {
                        cType = curCharger;
                        Q_EMIT chargerTypeChanged(cType);
                    }
                }
            }
        }
    }
}

void LomiriBatteryInfoPrivate::deviceAdded(const QString &path)
{
    QUPowerDeviceInterface *battery;
    battery = new QUPowerDeviceInterface(path,this);
    int batteryNumber = batteryCount();
    connect(battery,SIGNAL(propertyChanged(QString,QVariant)),
            this,SLOT(uPowerBatteryPropertyChanged(QString,QVariant)));

    if (battery->type() == 2) {
        batteryMap.insert(++batteryNumber,battery->getProperties());
    }
}

void LomiriBatteryInfoPrivate::deviceRemoved(const QString &path)
{
    QUPowerDeviceInterface *battery;
    battery = new QUPowerDeviceInterface(path,this);

    int foundBattery = 0;
    QMapIterator<int, QVariantMap> i(batteryMap);
     while (i.hasNext()) {
         i.next();
         if (i.value().value(QStringLiteral("NativePath")).toString()
                 == battery->nativePath()) {
             foundBattery = i.key();
             break;
         }
     }

    bool validBefore = isValid();
    disconnect(battery,SIGNAL(propertyChanged(QString,QVariant)),
            this,SLOT(uPowerBatteryPropertyChanged(QString,QVariant)));
    if (battery->type() == 2) {
        batteryMap.remove(foundBattery);
    }
    bool validNow = isValid();
    if (validBefore != validNow)
        Q_EMIT validChanged(validNow);
}

QT_END_NAMESPACE
