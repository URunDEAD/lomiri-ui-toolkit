/*
 * Copyright 2015 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TOUCHOWNERSHIPEVENT_P_H
#define TOUCHOWNERSHIPEVENT_P_H

#include <QtCore/QEvent>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriGestures-Qt6/lomirigesturesglobal.h>
#else
#include <LomiriGestures/lomirigesturesglobal.h>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
class QEventPoint;
class QPointerEvent;
#endif

UG_NAMESPACE_BEGIN
/*
   When an item get an ownership event for a touch it can grab/steal that touch
   with a clean conscience.
 */
class LOMIRIGESTURES_EXPORT TouchOwnershipEvent : public QEvent
{
public:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    TouchOwnershipEvent(int touchId,
                        bool gained,
                        QPointerEvent * pointerEvent,
                        const QEventPoint * point);
#else
    TouchOwnershipEvent(int touchId, bool gained);
#endif

    static Type touchOwnershipEventType();

    /*
      Whether ownership was gained (true) or lost (false)
     */
    bool gained() const { return m_gained; }

    /*
        Id of the touch whose ownership was granted.
     */
    int touchId() const { return m_touchId; }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    /*
      QPointerEvent which causes this event to be sent. May be `nullptr` if this
      event is not caused by a QPointerEvent.
     */
    QPointerEvent * pointerEvent() const { return m_pointerEvent; }

    /*
      A QEventPoint which refers to the touch id. This point is guaranteed to
      be inside `event()`. May be `nullptr` if `event()` is a `nullptr`.
     */
    const QEventPoint * point() const { return m_point; }
#endif

private:
    static Type m_touchOwnershipType;
    int m_touchId;
    bool m_gained;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QPointerEvent * m_pointerEvent;
    const QEventPoint * m_point;
#endif
};

UG_NAMESPACE_END

#endif // TOUCHOWNERSHIPEVENT_P_H
