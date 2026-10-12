/*
 * Copyright 2025 UBports Foundation
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
 *
 * Author: William Desportes <williamdes@wdes.fr>
 */

import QtQuick 2.0
import QtTest 1.0
import Lomiri.Test 1.0

LomiriTestCase {
    name: "UnitConverterTests"
    when: windowShown

    Loader {
        id: main
        source: "./unit-converter/unit-converter.qml"
    }

    function test_clear_action() {
        var action = findChildWithProperty(main, "iconName", "toolkit_input-clear");
        compare("Clear", action.text);
        action.trigger()
    }

    function test_convert() {
        var inchText = findChildWithProperty(main, "text", "Inch");
        compare("Inch", inchText.text);
        var inchTxtField = inchText.parent.children[1];
        compare("0.0", inchTxtField.text);
        inchTxtField.forceActiveFocus();
        typeString("14");
        compare("140.0", inchTxtField.text);
        var meterText = findChildWithProperty(main, "text", "Meter");
        compare("Meter", meterText.text);
        var meterTxtField = meterText.parent.children[1];
        compare("3.556", meterTxtField.text);
        var action = findChildWithProperty(main, "iconName", "toolkit_input-clear");
        compare("Clear", action.text);
        action.trigger();
        compare("0.0", inchTxtField.text);
        compare("0.0", meterTxtField.text);
    }
}
