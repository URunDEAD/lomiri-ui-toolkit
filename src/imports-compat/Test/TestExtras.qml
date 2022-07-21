/*
 * Copyright 2014 Canonical Ltd.
 * Copyright (C) 2022 UBports Foundation.
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

pragma Singleton

import QtQml 2.12
import QtQuick 2.12
import Lomiri.Test 1.3 as LT
// DeprecationPrinter
import Lomiri.Components.Private 1.3

Item {
    readonly property bool touchPresent: LT.TestExtras.touchPresent

    function openGLflavor(...args) {
        return LT.TestExtras.openGLflavor(...args);
    }

    function cpuArchitecture(...args) {
        return LT.TestExtras.cpuArchitecture(...args);
    }

    function touchDevicePresent(...args) {
        return LT.TestExtras.touchDevicePresent(...args);
    }

    function registerTouchDevice(...args) {
        return LT.TestExtras.registerTouchDevice(...args);
    }

    function touchPress(...args) {
        return LT.TestExtras.touchPress(...args);
    }

    function touchRelease(...args) {
        return LT.TestExtras.touchRelease(...args);
    }

    function touchClick(...args) {
        return LT.TestExtras.touchClick(...args);
    }

    function touchLongPress(...args) {
        return LT.TestExtras.touchLongPress(...args);
    }

    function touchDoubleClick(...args) {
        return LT.TestExtras.touchDoubleClick(...args);
    }

    function touchMove(...args) {
        return LT.TestExtras.touchMove(...args);
    }

    function touchDrag(...args) {
        return LT.TestExtras.touchDrag(...args);
    }

    function mouseDrag(...args) {
        return LT.TestExtras.mouseDrag(...args);
    }

    function removeTimeConstraintsFromSwipeArea(...args) {
        return LT.TestExtras.removeTimeConstraintsFromSwipeArea(...args);
    }

    Component.onCompleted: {
        DeprecationPrinter.printDeprecation(
            DeprecationPrinter.TEST);
    }
}
