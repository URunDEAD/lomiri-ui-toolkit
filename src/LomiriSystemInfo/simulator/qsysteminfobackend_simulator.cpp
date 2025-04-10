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

#include "qsysteminfobackend_simulator_p.h"

#include <QMutex>

QT_BEGIN_NAMESPACE

LomiriBatteryInfoSimulatorBackend *LomiriBatteryInfoSimulatorBackend::globalSimulatorBackend = 0;

// LomiriBatteryInfoSimulatorBackend

LomiriBatteryInfoSimulatorBackend::LomiriBatteryInfoSimulatorBackend(QObject *parent)
    : QObject(parent)
{
    data.index = 0;
    data.currentFlow = 0;
    data.cycleCount = -1;
    data.maximumCapacity = -1;
    data.remainingCapacity = -1;
    data.remainingChargingTime = -1;
    data.voltage = -1;
    data.chargingState = LomiriBatteryInfo::UnknownChargingState;
    data.chargerType = LomiriBatteryInfo::UnknownCharger;
    data.levelStatus = LomiriBatteryInfo::LevelUnknown;
    data.health = LomiriBatteryInfo::HealthUnknown;
}

LomiriBatteryInfoSimulatorBackend::~LomiriBatteryInfoSimulatorBackend()
{
}

LomiriBatteryInfoSimulatorBackend *LomiriBatteryInfoSimulatorBackend::getSimulatorBackend()
{
    static QMutex mutex;

    mutex.lock();
    if (!globalSimulatorBackend)
        globalSimulatorBackend = new LomiriBatteryInfoSimulatorBackend();
    mutex.unlock();

    return globalSimulatorBackend;
}


int LomiriBatteryInfoSimulatorBackend::getBatteryCount()
{
    return 1;
}

int LomiriBatteryInfoSimulatorBackend::getBatteryIndex() const
{
    return data.index;
}

int LomiriBatteryInfoSimulatorBackend::getLevel(int battery)
{
    if (battery == 0) {
        int maxCapacity = getMaximumCapacity(battery);
        int remCapacity = getRemainingCapacity(battery);

        if (maxCapacity == 0)
            return -1;

        return remCapacity * 100 / maxCapacity;
    }

    return -1;
}

int LomiriBatteryInfoSimulatorBackend::getCurrentFlow(int battery)
{
    if (battery == 0)
        return data.currentFlow;
    return -1;
}

int LomiriBatteryInfoSimulatorBackend::getCycleCount(int battery)
{
    if (battery == 0)
        return data.cycleCount;

    return -1;
}

int LomiriBatteryInfoSimulatorBackend::getMaximumCapacity(int battery)
{
    if (battery == 0)
        return data.maximumCapacity;
    return -1;
}

int LomiriBatteryInfoSimulatorBackend::getRemainingCapacity(int battery)
{
    if (battery == 0)
        return data.remainingCapacity;
    return -1;
}

int LomiriBatteryInfoSimulatorBackend::getRemainingChargingTime(int battery)
{
    if (battery == 0)
        return data.remainingChargingTime;
    return -1;
}

int LomiriBatteryInfoSimulatorBackend::getVoltage(int battery)
{
    if (battery == 0)
        return data.voltage;
    return -1;
}

LomiriBatteryInfo::ChargingState LomiriBatteryInfoSimulatorBackend::getChargingState(int battery)
{
    if (battery == 0)
        return data.chargingState;

    return LomiriBatteryInfo::UnknownChargingState;
}

LomiriBatteryInfo::ChargerType LomiriBatteryInfoSimulatorBackend::getChargerType()
{
    return data.chargerType;
}

LomiriBatteryInfo::LevelStatus LomiriBatteryInfoSimulatorBackend::getLevelStatus(int battery)
{
    if (battery == 0)
        return data.levelStatus;

    return LomiriBatteryInfo::LevelUnknown;
}

LomiriBatteryInfo::Health LomiriBatteryInfoSimulatorBackend::getHealth(int battery)
{
    if (battery == 0)
        return data.health;

    return LomiriBatteryInfo::HealthUnknown;
}

float LomiriBatteryInfoSimulatorBackend::getTemperature(int battery)
{
    if (battery == 0)
        return data.temperature;

    return -1.0f;
}

void LomiriBatteryInfoSimulatorBackend::setBatteryIndex(int batteryIndex)
{
    if (data.index != batteryIndex) {
        data.index = batteryIndex;
        Q_EMIT batteryIndexChanged(data.index);
    }
}

void LomiriBatteryInfoSimulatorBackend::setCurrentFlow(int flow)
{
    if (data.currentFlow != flow) {
        data.currentFlow = flow;
        Q_EMIT currentFlowChanged(flow);
    }
}

void LomiriBatteryInfoSimulatorBackend::setCycleCount(int cycleCount)
{
    if (data.cycleCount != cycleCount) {
        data.cycleCount = cycleCount;
        Q_EMIT cycleCountChanged(cycleCount);
    }
}

void LomiriBatteryInfoSimulatorBackend::setMaximumCapacity(int capacity)
{
    if (data.maximumCapacity != capacity) {
        int levelBefore = getLevel(0);
        data.maximumCapacity = capacity;
        int levelNow = getLevel(0);
        if (levelBefore != levelNow) {
            Q_EMIT levelChanged(levelNow);
        }
    }
}

void LomiriBatteryInfoSimulatorBackend::setRemainingCapacity(int capacity)
{
    if (data.remainingCapacity != capacity) {
        int levelBefore = getLevel(0);
        data.remainingCapacity = capacity;
        Q_EMIT remainingCapacityChanged(capacity);
        int levelNow = getLevel(0);
        if (levelBefore != levelNow) {
            Q_EMIT levelChanged(levelNow);
        }
    }
}

void LomiriBatteryInfoSimulatorBackend::setVoltage(int vol)
{
    if (data.voltage != vol) {
        data.voltage = vol;
        Q_EMIT voltageChanged(vol);
    }
}

void LomiriBatteryInfoSimulatorBackend::setRemainingChargingTime(int time)
{
    if (data.remainingChargingTime != time) {
        data.remainingChargingTime = time;
        Q_EMIT remainingChargingTimeChanged(time);
    }
}

void LomiriBatteryInfoSimulatorBackend::setChargingState(LomiriBatteryInfo::ChargingState state)
{
    if (data.chargingState != state) {
        data.chargingState = state;
        Q_EMIT chargingStateChanged(state);
    }
}

void LomiriBatteryInfoSimulatorBackend::setChargerType(LomiriBatteryInfo::ChargerType type)
{
    if (data.chargerType != type) {
        data.chargerType = type;
        Q_EMIT chargerTypeChanged(type);
    }
}

void LomiriBatteryInfoSimulatorBackend::setLevelStatus(LomiriBatteryInfo::LevelStatus levelStatus)
{
    if (data.levelStatus != levelStatus) {
        data.levelStatus = levelStatus;
        Q_EMIT levelStatusChanged(levelStatus);
    }
}

void LomiriBatteryInfoSimulatorBackend::setHealth(LomiriBatteryInfo::Health health)
{
    if (data.health != health) {
        data.health = health;
        Q_EMIT healthChanged(health);
    }
}

void LomiriBatteryInfoSimulatorBackend::setTemperature(float temperature)
{
    if (!qFuzzyCompare(data.temperature, temperature)) {
        data.temperature = temperature;
        Q_EMIT temperatureChanged(temperature);
    }
}

QT_END_NAMESPACE
