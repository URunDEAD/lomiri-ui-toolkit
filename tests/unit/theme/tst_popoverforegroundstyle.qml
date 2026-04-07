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
import Lomiri.Components.Themes.Ambiance
import Lomiri.Components.Popups

TestCase {
    name: "PopupsPopoverAPI"

    function test_show() {
        popOver.show()
    }

    Popover {
        id: popOver

        foregroundStyle: testForegroundStyle
        Text {
            text: "Hello Popover!"
        }
    }

    Component {
        id: testForegroundStyle
        PopoverForegroundStyle {
        }
    }
}
