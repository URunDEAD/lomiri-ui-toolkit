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

#include "qsysteminfo_simulator_p.h"
#include "qsysteminfobackend_simulator_p.h"
#include "qsysteminfoconnection_simulator_p.h"

#include <QtCore/qmetaobject.h>

QT_BEGIN_NAMESPACE

// LomiriBatteryInfoSimulator

LomiriBatteryInfoSimulator::LomiriBatteryInfoSimulator(LomiriBatteryInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , batteryInfoSimulatorBackend(LomiriBatteryInfoSimulatorBackend::getSimulatorBackend())
{
    SystemInfoConnection::ensureSimulatorConnection();
}

LomiriBatteryInfoSimulator::LomiriBatteryInfoSimulator(int batteryIndex, LomiriBatteryInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , batteryInfoSimulatorBackend(LomiriBatteryInfoSimulatorBackend::getSimulatorBackend())
{
    SystemInfoConnection::ensureSimulatorConnection();
    setBatteryIndex(batteryIndex);
}

LomiriBatteryInfoSimulator::~LomiriBatteryInfoSimulator()
{
}

int LomiriBatteryInfoSimulator::batteryCount()
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getBatteryCount();

    return -1;
}

int LomiriBatteryInfoSimulator::batteryIndex() const
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getBatteryIndex();

    return -1;
}

bool LomiriBatteryInfoSimulator::isValid()
{
    // valid if the index < total count.
    return (batteryIndex() >= 0) && (batteryIndex() < batteryCount());
}

void LomiriBatteryInfoSimulator::setBatteryIndex(int batteryIndex)
{
    if (batteryInfoSimulatorBackend)
        batteryInfoSimulatorBackend->setBatteryIndex(batteryIndex);
}

int LomiriBatteryInfoSimulator::level()
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getLevel(batteryInfoSimulatorBackend->getBatteryIndex());

    return -1;
}

int LomiriBatteryInfoSimulator::currentFlow(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getCurrentFlow(battery);

    return 0;
}

int LomiriBatteryInfoSimulator::currentFlow()
{
    return currentFlow(batteryInfoSimulatorBackend->getBatteryIndex());
}

int LomiriBatteryInfoSimulator::cycleCount()
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getCycleCount(batteryInfoSimulatorBackend->getBatteryIndex());

    return -1;
}

int LomiriBatteryInfoSimulator::maximumCapacity(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getMaximumCapacity(battery);

    return -1;
}

int LomiriBatteryInfoSimulator::maximumCapacity()
{
    return maximumCapacity(batteryInfoSimulatorBackend->getBatteryIndex());
}

int LomiriBatteryInfoSimulator::remainingCapacity(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getRemainingCapacity(battery);

    return -1;
}

int LomiriBatteryInfoSimulator::remainingCapacity()
{
    return remainingCapacity(batteryInfoSimulatorBackend->getBatteryIndex());
}

int LomiriBatteryInfoSimulator::remainingChargingTime(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getRemainingChargingTime(battery);

    return -1;
}

int LomiriBatteryInfoSimulator::remainingChargingTime()
{
    return remainingChargingTime(batteryInfoSimulatorBackend->getBatteryIndex());
}

int LomiriBatteryInfoSimulator::voltage(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getVoltage(battery);

    return -1;
}

int LomiriBatteryInfoSimulator::voltage()
{
    return voltage(batteryInfoSimulatorBackend->getBatteryIndex());
}

LomiriBatteryInfo::ChargerType LomiriBatteryInfoSimulator::chargerType()
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getChargerType();

    return LomiriBatteryInfo::UnknownCharger;
}

LomiriBatteryInfo::ChargingState LomiriBatteryInfoSimulator::chargingState(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getChargingState(battery);

    return LomiriBatteryInfo::UnknownChargingState;
}

LomiriBatteryInfo::ChargingState LomiriBatteryInfoSimulator::chargingState()
{
    return chargingState(batteryInfoSimulatorBackend->getBatteryIndex());
}

LomiriBatteryInfo::LevelStatus LomiriBatteryInfoSimulator::levelStatus(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getLevelStatus(battery);

    return LomiriBatteryInfo::LevelUnknown;
}

LomiriBatteryInfo::LevelStatus LomiriBatteryInfoSimulator::levelStatus()
{
    return levelStatus(batteryInfoSimulatorBackend->getBatteryIndex());
}

LomiriBatteryInfo::Health LomiriBatteryInfoSimulator::health()
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getHealth(batteryInfoSimulatorBackend->getBatteryIndex());

    return LomiriBatteryInfo::HealthUnknown;
}

float LomiriBatteryInfoSimulator::temperature()
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getTemperature(batteryInfoSimulatorBackend->getBatteryIndex());

    return -1.0f;
}

extern QMetaMethod proxyToSourceSignal(const QMetaMethod &, QObject *);

void LomiriBatteryInfoSimulator::connectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod batteryCountChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::batteryCountChanged);
    static const QMetaMethod chargerTypeChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::chargerTypeChanged);
    static const QMetaMethod chargingStateChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::chargingStateChanged);
    static const QMetaMethod currentFlowChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::currentFlowChanged);
    static const QMetaMethod remainingCapacityChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::remainingCapacityChanged);
    static const QMetaMethod remainingChargingTimeChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::remainingChargingTimeChanged);
    static const QMetaMethod voltageChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::voltageChanged);
    static const QMetaMethod levelStatusChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::levelStatusChanged);
    static const QMetaMethod healthChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::healthChanged);
    static const QMetaMethod temperatureChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::temperatureChanged);

    if (batteryInfoSimulatorBackend && (signal == batteryCountChangedSignal
                                        || signal == currentFlowChangedSignal
                                        || signal == voltageChangedSignal
                                        || signal == remainingCapacityChangedSignal
                                        || signal == remainingChargingTimeChangedSignal
                                        || signal == chargerTypeChangedSignal
                                        || signal == chargingStateChangedSignal
                                        || signal == levelStatusChangedSignal
                                        || signal == healthChangedSignal
                                        || signal == temperatureChangedSignal)) {
        QMetaMethod sourceSignal = proxyToSourceSignal(signal, batteryInfoSimulatorBackend);
        connect(batteryInfoSimulatorBackend, sourceSignal, this, signal);
    }
}

void LomiriBatteryInfoSimulator::disconnectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod batteryCountChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::batteryCountChanged);
    static const QMetaMethod chargerTypeChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::chargerTypeChanged);
    static const QMetaMethod chargingStateChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::chargingStateChanged);
    static const QMetaMethod currentFlowChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::currentFlowChanged);
    static const QMetaMethod remainingCapacityChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::remainingCapacityChanged);
    static const QMetaMethod remainingChargingTimeChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::remainingChargingTimeChanged);
    static const QMetaMethod voltageChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::voltageChanged);
    static const QMetaMethod levelStatusChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::levelStatusChanged);
    static const QMetaMethod healthChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::healthChanged);
    static const QMetaMethod temperatureChangedSignal = QMetaMethod::fromSignal(&LomiriBatteryInfoSimulator::temperatureChanged);

    if (batteryInfoSimulatorBackend && (signal == batteryCountChangedSignal
                                        || signal == currentFlowChangedSignal
                                        || signal == voltageChangedSignal
                                        || signal == remainingCapacityChangedSignal
                                        || signal == remainingChargingTimeChangedSignal
                                        || signal == chargerTypeChangedSignal
                                        || signal == chargingStateChangedSignal
                                        || signal == levelStatusChangedSignal
                                        || signal == healthChangedSignal
                                        || signal == temperatureChangedSignal)) {
        QMetaMethod sourceSignal = proxyToSourceSignal(signal, batteryInfoSimulatorBackend);
        disconnect(batteryInfoSimulatorBackend, sourceSignal, this, signal);
    }
}

QT_END_NAMESPACE
