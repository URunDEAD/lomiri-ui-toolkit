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

#include "touchownershipevent_p.h"

UG_NAMESPACE_BEGIN

QEvent::Type TouchOwnershipEvent::m_touchOwnershipType = (QEvent::Type)-1;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
TouchOwnershipEvent::TouchOwnershipEvent(int touchId,
                                         bool gained,
                                         QPointerEvent * pointerEvent,
                                         const QEventPoint * point)
#else
TouchOwnershipEvent::TouchOwnershipEvent(int touchId, bool gained)
#endif
    : QEvent(touchOwnershipEventType())
    , m_touchId(touchId)
    , m_gained(gained)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    , m_pointerEvent(pointerEvent)
    , m_point(point)
#endif
{
}

QEvent::Type TouchOwnershipEvent::touchOwnershipEventType()
{
    if (m_touchOwnershipType == (QEvent::Type)-1) {
        m_touchOwnershipType = (QEvent::Type)registerEventType();
    }

    return m_touchOwnershipType;
}

UG_NAMESPACE_END
