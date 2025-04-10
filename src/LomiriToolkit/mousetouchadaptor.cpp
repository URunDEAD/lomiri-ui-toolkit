/*
 * Copyright (C) 2013,2016 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Daniel d'Andrada <daniel.dandrada@canonical.com>
 *              Zsombor Egri <zsomboir.egri@canonical.com>
 */

#include "mousetouchadaptor_p_p.h"

#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/qpa/qplatformnativeinterface.h>
#include <QtGui/qpa/qwindowsysteminterface.h>

UT_NAMESPACE_BEGIN

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QInputDevice *MouseTouchAdaptor::m_touchDevice = nullptr;
#else
QTouchDevice *MouseTouchAdaptor::m_touchDevice = nullptr;
#endif

MouseTouchAdaptorPrivate::~MouseTouchAdaptorPrivate()
{
    QCoreApplication::instance()->removeNativeEventFilter(this);
}

/*!
 * \qmltype MouseTouchAdaptor
 * \instantiates MouseTouchAdaptor
 * \inqmlmodule Lomiri.Test 1.0
 * \ingroup lomiri-test
 * \brief Singleton type turning mouse events into single finger touch events.
 *
 * When enabled, mouse events will be translated into single finger touch events.
 * Being a singleton, the feature must be enabled explicitly either on component
 * completion or through a binding.
 * \qml
 * Binding {
 *     target: MouseTouchAdaptor
 *     property: "enabled"
 *     value: true
 * }
 * \endqml
 *
 */
MouseTouchAdaptor::MouseTouchAdaptor(QObject *parent)
    :
      QObject(*(new X11MouseTouchAdaptorPrivate), parent)
{
    registerTouchDevice();
    Q_D(MouseTouchAdaptor);
    d->init();
}

// registers a test touch device, returns true if a device was found/registered
bool MouseTouchAdaptor::registerTouchDevice()
{
    // check if there is any touch device registered in the system
    if (!m_touchDevice) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QList<const QInputDevice*> touchDevices = QInputDevice::devices();
        for (const QInputDevice *device : touchDevices) {
            if (device->type() == QInputDevice::DeviceType::TouchScreen) {
                m_touchDevice = const_cast<QInputDevice*>(device);
#else
        QList<const QTouchDevice*> touchDevices = QTouchDevice::devices();
        Q_FOREACH(const QTouchDevice *device, touchDevices) {
            if (device->type() == QTouchDevice::TouchScreen) {
                m_touchDevice = const_cast<QTouchDevice*>(device);
#endif
                return true;
            }
        }
    }
    // if none, register one
    if (!m_touchDevice) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        m_touchDevice = new QInputDevice;
        m_touchDevice->setType(QInputDevice::DeviceType::TouchScreen);
#else
        m_touchDevice = new QTouchDevice;
        m_touchDevice->setType(QTouchDevice::TouchScreen);
#endif
        QWindowSystemInterface::registerTouchDevice(m_touchDevice);
        return true;
    }
    return false;
}

QWindow *MouseTouchAdaptorPrivate::findQWindowWithXWindowID(WId windowId)
{
    QWindowList windowList = QGuiApplication::topLevelWindows();
    QWindow *foundWindow = nullptr;

    int i = 0;
    while (!foundWindow && i < windowList.count()) {
        QWindow *window = windowList[i];
        if (window->winId() == windowId) {
            foundWindow = window;
        } else {
            ++i;
        }
    }

    Q_ASSERT(foundWindow);
    return foundWindow;
}

/*!
 * \qmlproperty bool MouseTouchAdaptor::enabled
 * Enables the mouse to touch conversion functionality. Defaults to true.
 */
bool MouseTouchAdaptorPrivate::isEnabled() const
{
    return enabled;
}
void MouseTouchAdaptorPrivate::setEnabled(bool value)
{
    Q_UNUSED(value);
    qWarning() << "MouseTouchAdaptor not available on this architecture, thus cannot be enabled.";
}

UT_NAMESPACE_END

#include "moc_mousetouchadaptor_p.cpp"
