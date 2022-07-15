/*
 * Copyright (C) 2013,2016 Canonical Ltd.
 * Copyrigit (C) 2022 UBports Foundation.
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
 */

pragma Singleton

import QtQuick 2.12
import Lomiri.Test 1.3 as LT

QtObject {
    /*
     * Since QML doesn't support aliasing singleton, and this property is
     * read-write, we need to manually sync it. Further, since `enabled`
     * property exists on Connections type, we can't listen to
     * LT.MouseTouchAdaptor.enabled directly and need a redirection point.
     */
    property bool enabled: LT.MouseTouchAdaptor.enabled

    onEnabledChanged: {
        if (LT.MouseTouchAdaptor.enabled !== enabled)
            LT.MouseTouchAdaptor.enabled = enabled;
    }

    readonly property bool enabledPriv: LT.MouseTouchAdaptor.enabled
    onEnabledPrivChanged: {
        if (enabled !== LT.MouseTouchAdaptor.enabled)
            enabled = LT.MouseTouchAdaptor.enabled;
    }
}
