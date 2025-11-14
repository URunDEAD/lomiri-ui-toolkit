/****************************************************************************
**
** Copyright (C) 2018 Canonical, Ltd. and/or its subsidiary(-ies).
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

#include "lomiriinputinfo.h"

#include "lomiriinputinfomanager_p.h"

Q_GLOBAL_STATIC(LomiriInputInfoManagerPrivate, inputDeviceManagerPrivate)

#if defined(Q_OS_LINUX)
#if !defined(QT_NO_UDEV)
#include "linux/lomiriinputinfomanagerudev_p.h"
#endif

#if !defined(QT_NO_MIR)
#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>
#include "linux/lomiriinputinfomanagermir_p.h"
#endif

#if !defined(QT_NO_UDEV)
Q_GLOBAL_STATIC(LomiriInputInfoManagerUdev, inputDeviceManagerUdev)
#endif
#if !defined(QT_NO_MIR)
static void not_owning_connection(MirConnection*) {}
LomiriInputInfoManagerMir::MirConnectionPtr attempt_to_connect_to_mir()
{
    auto nativeInterface = QGuiApplication::platformNativeInterface();

    if (nativeInterface) {
        auto connection = static_cast<MirConnection*>(nativeInterface->nativeResourceForIntegration("mirconnection"));
        return LomiriInputInfoManagerMir::MirConnectionPtr{connection, &not_owning_connection};
    } else {
        auto connection = mir_connect_sync(NULL, "qtsystems-plugin");
        if (mir_connection_is_valid(connection))
            return LomiriInputInfoManagerMir::MirConnectionPtr{connection, &mir_connection_release};
        else
            return LomiriInputInfoManagerMir::MirConnectionPtr{connection, &not_owning_connection};
    }
}

Q_GLOBAL_STATIC_WITH_ARGS(LomiriInputInfoManagerMir, inputDeviceManagerMir, (attempt_to_connect_to_mir()))
#endif
#endif

QT_BEGIN_NAMESPACE

/*!
    \internal
*/
LomiriInputInfoManagerPrivate * LomiriInputInfoManagerPrivate::instance()
{
#ifndef QT_NO_MIR
    if (inputDeviceManagerMir.exists()) {
        return inputDeviceManagerMir();
    } else {
        auto connection_attempt = attempt_to_connect_to_mir();
        if (connection_attempt && mir_connection_is_valid(connection_attempt.get())) {
            connection_attempt.reset();
            return inputDeviceManagerMir();
        }
    }
#endif
#ifndef QT_NO_UDEV
    return inputDeviceManagerUdev();
#endif
    return inputDeviceManagerPrivate();
}

/*!
    \internal
*/
LomiriInputDevicePrivate::LomiriInputDevicePrivate(QObject *parent) :
    QObject(parent),
    type(LomiriInputDevice::UnknownType)
{
    qRegisterMetaType<LomiriInputDevice::InputType>();
    qRegisterMetaType<LomiriInputDevice::InputTypeFlags>();
}


/*!
    \class LomiriInputDevice
    \inmodule QtSystemInfo
    \brief The LomiriInputDevice class provides various information about one input device.
    \ingroup systeminfo

*/
/*!
    \enum InputType::InputType
    This enum describes the type of input.

    \value UnknownType         The input type is unknown.
    \value Button              Input is a button.
    \value Mouse               Input is a mouse.
    \value TouchPad            Input is a touch pad.
    \value TouchScreen         Input is a touch screen.
    \value Keyboard            Input is a keyboard
    \value Switch              Input is a switch
*/

/*!
    Constructs a \l LomiriInputDevice object with the given \a parent.
*/
LomiriInputDevice::LomiriInputDevice(QObject *parent) :
    QObject(parent),
    d_ptr(new LomiriInputDevicePrivate(this))
{
}

/*!
    \property LomiriInputDevice::properties
    \brief The properties of this input device.

    This property contains a variant map of all properties known for this input device.
    Name and identifier properties are required.

  \sa name(), identifier(), buttons(), switches(), relativeAxes(), absoluteAxes(), type()

*/
QVariantMap LomiriInputDevice::properties()
{
    return deviceProperties;
}

/*
    Returns the name of this input device.
 */
QString LomiriInputDevice::name() const
{
    return d_ptr->name;
}

/*
    Sets the name of this input device to \b name.
 */
void LomiriInputDevice::setName(const QString &name)
{
    deviceProperties.insert(QStringLiteral("name"),name);
    d_ptr->name = name;
}

/*
    Returns the identifier of this device.
 */
QString LomiriInputDevice::identifier() const
{
    return d_ptr->identifier;
}

/*
    Sets the identifier of this device to /b id.
 */
void LomiriInputDevice::setIdentifier(const QString &id)
{
    d_ptr->identifier = id;
    deviceProperties.insert(QStringLiteral("identifier"),id);
}

/*
    Returns the number of buttons this device has.
 */
QList <int> LomiriInputDevice::buttons() const
{
    return d_ptr->buttons;
}

/*
    \internal
 */
void LomiriInputDevice::addButton(int buttonCode)
{
    d_ptr->buttons.append(buttonCode);
    deviceProperties.insert(QStringLiteral("buttons"),QVariant::fromValue(d_ptr->buttons));
}

/*
    Returns the number of switches of this device.
 */
QList <int> LomiriInputDevice::switches() const
{
    return d_ptr->switches;
}

/*
    \internal
 */
void LomiriInputDevice::addSwitch(int switchCode)
{
    d_ptr->switches.append(switchCode);
    deviceProperties.insert(QStringLiteral("switches"),QVariant::fromValue(d_ptr->switches));
}

/*
    Returns a list of the relative axes of this device
 */
QList <int> LomiriInputDevice::relativeAxes() const
{
    return d_ptr->relativeAxes;
}

/*
    \internal
 */
void LomiriInputDevice::addRelativeAxis(int axisCode)
{
    d_ptr->relativeAxes.append(axisCode);
    deviceProperties.insert(QStringLiteral("rAxis"),QVariant::fromValue(d_ptr->relativeAxes));
}

/*
    Returns a list of the absolute axes of this device
 */
QList <int> LomiriInputDevice::absoluteAxes() const
{
    return d_ptr->absoluteAxes;
}

/*
    \internal
 */
void LomiriInputDevice::addAbsoluteAxis(int axisCode)
{
    d_ptr->absoluteAxes.append(axisCode);
    deviceProperties.insert(QStringLiteral("aAxis"),QVariant::fromValue(d_ptr->absoluteAxes));
}

/*
    Returns a LomiriInputDevice::InputTypeFlags of all the types of types.
 */
LomiriInputDevice::InputTypeFlags LomiriInputDevice::types() const
{
    return d_ptr->type;
}

/*
    \internal
 */
void LomiriInputDevice::setTypes(LomiriInputDevice::InputTypeFlags type)
{
    d_ptr->type = type;
    deviceProperties.insert(QStringLiteral("types"),QVariant::fromValue(type));
}

/*!
    \class LomiriInputInfoManager
    \inmodule QtSystemInfo

    \brief The LomiriInputInfoManager class manages input devices.

*/
LomiriInputInfoManager::LomiriInputInfoManager(QObject *parent) :
    QObject(parent),
    d_ptr(LomiriInputInfoManagerPrivate::instance()),
    currentFilter(LomiriInputDevice::Button | LomiriInputDevice::Mouse
                   | LomiriInputDevice::TouchPad | LomiriInputDevice::TouchScreen
                   | LomiriInputDevice::Keyboard
                   | LomiriInputDevice::Switch),
   filteredCount(0)
{

    connect(d_ptr, &LomiriInputInfoManagerPrivate::deviceAdded,this,&LomiriInputInfoManager::addedDevice);
    connect(d_ptr, &LomiriInputInfoManagerPrivate::deviceRemoved,this,&LomiriInputInfoManager::removedDevice);

    connect(d_ptr,SIGNAL(ready()),this,SLOT(privateReady()));

}

/*!
    Destroy the QSensorGestureManager
*/
LomiriInputInfoManager::~LomiriInputInfoManager()
{
}
/*!
    \fn LomiriInputInfoManager::ready()

    This signal is Q_EMITted when input device map and associated properties are ready to use.
*/
/*!
    \fn LomiriInputInfoManager::deviceAdded(LomiriInputDevice *inputDevice)

    This signal is Q_EMITted when a new input device is added.
*/
/*!
    \fn LomiriInputInfoManager::deviceRemoved(const QString &deviceId)

    This signal is Q_EMITted when a new input device is removed.
*/

/*
    \internal
 */
void LomiriInputInfoManager::privateReady()
{
    setFilter(currentFilter);
    Q_EMIT ready();
}

/*
    Returns a QMap of input devices using the currently set LomiriInputDevice::InputTypeFlags filter.
 */
QMap <QString, LomiriInputDevice *> LomiriInputInfoManager::deviceMap()
{
    return currentFilteredMap;
}

/*
    \internal
 */
void LomiriInputInfoManager::addedDevice(LomiriInputDevice *deviceInfo)
{
    Q_UNUSED(deviceInfo);
    int oldFilteredCount = filteredCount;

    setFilter(currentFilter);

    if (oldFilteredCount < filteredCount) {
        Q_EMIT deviceAdded(deviceInfo);
    }
}

/*
    \internal
 */
void LomiriInputInfoManager::removedDevice(const QString &deviceId)
{
    bool ok = false;
    if (currentFilteredMap.contains(deviceId))
        ok = true;
    setFilter(currentFilter);
    if (ok) {
        Q_EMIT deviceRemoved(deviceId);
    }
}

/*
    Returns the number of input devices with the currently set LomiriInputDevice::InputTypeFlags filter,
    if no filter is set, this returns number of all available input devices.
 */
int LomiriInputInfoManager::count() const
{
    int deviceCount = 0;
    if (currentFilter.testFlag(LomiriInputDevice::Button)) {
        deviceCount += count(static_cast< LomiriInputDevice::InputType >(LomiriInputDevice::Button));
    }
    if (currentFilter.testFlag(LomiriInputDevice::Mouse)) {
        deviceCount += count(static_cast< LomiriInputDevice::InputType >(LomiriInputDevice::Mouse));
    }
    if (currentFilter.testFlag(LomiriInputDevice::TouchPad)) {
        deviceCount += count(static_cast< LomiriInputDevice::InputType >(LomiriInputDevice::TouchPad));
    }
    if (currentFilter.testFlag(LomiriInputDevice::TouchScreen)) {
        deviceCount += count(static_cast< LomiriInputDevice::InputType >(LomiriInputDevice::TouchScreen));
    }
    if (currentFilter.testFlag(LomiriInputDevice::Keyboard)) {
        deviceCount += count(static_cast< LomiriInputDevice::InputType >(LomiriInputDevice::Keyboard));
    }
    if (currentFilter.testFlag(LomiriInputDevice::Switch)) {
        deviceCount += count(static_cast< LomiriInputDevice::InputType >(LomiriInputDevice::Switch));
    }
    return deviceCount;
}

/*
    \property LomiriInputInfoManager::count
    \brief The number of input devices of the type filter.

    Returns the number of input devices of the type \a filter, regardless of the filter
    that is currently set.
 */
/*!
    \fn LomiriInputInfoManager::countChanged(int count)

    This signal is Q_EMITted when the count of devices in regards to the current filter changes.
*/

int LomiriInputInfoManager::count(const LomiriInputDevice::InputType filter) const
{
    int dList = 0;
    QMapIterator<QString, LomiriInputDevice *> i(d_ptr->deviceMap);
    while (i.hasNext()) {
        i.next();
        if (i.value()->types().testFlag(filter)) {
            dList++;
        }
    }
    return dList;
}

/*
    \property LomiriInputInfoManager::filter
    \brief The currently set filter.

    Returns the currently set device filter.
    If none is set, default filter includes all LomiriInputDevice::InputType
 */

/*!
    \fn LomiriInputInfoManager::filterChanged(LomiriInputDevice::InputTypeFlags filterFlags)

    This signal is Q_EMITted when the filter property changes.
*/

LomiriInputDevice::InputTypeFlags LomiriInputInfoManager::filter()
{
    return currentFilter;
}

/*
 * Sets the current input device filter to \a filter, and updates device map.
 *
 * \sa deviceMap()
 */
void LomiriInputInfoManager::setFilter(LomiriInputDevice::InputTypeFlags filter)
{
    currentFilteredMap.clear();
    if (!filter) {
        filter = filter & (LomiriInputDevice::Button | LomiriInputDevice::Mouse
                | LomiriInputDevice::TouchPad | LomiriInputDevice::TouchScreen
                | LomiriInputDevice::Keyboard | LomiriInputDevice::Switch);
    }
    QMapIterator<QString, LomiriInputDevice *> i(d_ptr->deviceMap);
    while (i.hasNext()) {
        i.next();

        if (filter.testFlag(LomiriInputDevice::Button)
                && i.value()->types().testFlag(LomiriInputDevice::Button)) {
            currentFilteredMap.insert(i.key(),i.value());
        }
        if (filter.testFlag(LomiriInputDevice::Mouse)
                && i.value()->types().testFlag(LomiriInputDevice::Mouse)) {
            currentFilteredMap.insert(i.key(),i.value());
        }
        if (filter.testFlag(LomiriInputDevice::TouchPad)
                && i.value()->types().testFlag(LomiriInputDevice::TouchPad)) {
            currentFilteredMap.insert(i.key(),i.value());
        }
        if (filter.testFlag(LomiriInputDevice::TouchScreen)
                && i.value()->types().testFlag(LomiriInputDevice::TouchScreen)) {
            currentFilteredMap.insert(i.key(),i.value());
        }
        if (filter.testFlag(LomiriInputDevice::Keyboard)
                && i.value()->types().testFlag(LomiriInputDevice::Keyboard)) {
            currentFilteredMap.insert(i.key(),i.value());
        }
        if (filter.testFlag(LomiriInputDevice::Switch)
                && i.value()->types().testFlag(LomiriInputDevice::Switch)) {
            currentFilteredMap.insert(i.key(),i.value());
        }
    }

    if (filter != currentFilter) {
        currentFilter = filter;
        Q_EMIT filterChanged(filter);
    }

    if (currentFilteredMap.count() != filteredCount) {
        filteredCount = currentFilteredMap.count();
        Q_EMIT countChanged(filteredCount);
    }
}

/*
    \property LomiriInputInfoManager::lastAdded
    \brief The last added input device.

    Returns the last added input device.
 */
LomiriInputDevice *LomiriInputInfoManager::lastAdded()
{
   return currentFilteredMap.last();
}

QT_END_NAMESPACE
