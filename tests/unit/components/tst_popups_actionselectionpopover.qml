/*
 * Copyright 2012 Canonical Ltd.
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

import QtQuick
import QtTest
import Lomiri.Components
import Lomiri.Components.Popups

TestCase {
    name: "PopupsActionSelectionPopoverAPI"

    ActionSelectionPopover {
        id: popOver
    }

    function test_target() {
        compare(popOver.target, null, "Target is undefined by default");
    }

    function test_actions() {
        compare(popOver.actions, undefined, "Actions are undefined by default");
    }

    function test_delegate() {
        expectFail("", "Delegate should not be undefined by default");
        compare(popOver.delegate, undefined, "popOver.delegate");
    }
}
