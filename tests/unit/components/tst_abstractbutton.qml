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

Item {
    width: units.gu(40)
    height: units.gu(40)

    AbstractButton {
        id: absButton
        width: 100
        height: 100
        Rectangle {
            anchors.fill: parent
            color: "lightblue"
        }
        SignalSpy {
            id: signalSpy
            target: parent
        }
    }

    Action {
        id: action1
        property int triggerCount: 0
        onTriggered: triggerCount++
    }

    TestCase {
        name: "AbstractButtonAPI"

        function test_action() {
            compare(absButton.action, null, "Action is null by default");
            absButton.action = action1;
            compare(absButton.action, action1, "Action can be set");
            var numTriggers = action1.triggerCount;
            // The mouseClick() simulation makes the mousearea believe that the
            // cursor is hovering over it, which is problematic for test_hovered(),
            // so we disable hover detection here.
            absButton.__mouseArea.hoverEnabled = false;
            mouseClick(absButton);
            absButton.__mouseArea.hoverEnabled = true;
            compare(action1.triggerCount, numTriggers + 1, "Button clicked triggers action");
            absButton.action = null;
        }

        function test_hovered() {
            compare(absButton.hovered, false, "Hovered is boolean and false by default");
        }

        function test_pressed() {
            compare(absButton.pressed, false, "Pressed is boolean and false by default");
        }

        function test_signal_clicked() {
            signalSpy.signalName = "clicked";
            compare(signalSpy.valid, true, "clicked signal exists");
        }

        function test_signal_pressAndHold() {
            signalSpy.signalName = "pressAndHold";
            compare(signalSpy.valid, true, "pressAndHold signal exists");
        }
    }
}
