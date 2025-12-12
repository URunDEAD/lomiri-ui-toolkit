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

import QtQuick 2.0
import Lomiri.Components 0.1

Column {
    Slider {
       id: slider
       minimumValue: 0
       maximumValue: 11
    }
    Button {
        text: "live"
        onClicked: {
            slider.live = !slider.live;
        }
    }
    Button {
        text: "minmax_values"
        onClicked: {
            slider.minimumValue = slider.minimumValue + 1;
            slider.maximumValue = slider.maximumValue + 10;
        }
    }
    Button {
        text: "reset values"
        onClicked: {
            slider.value = -10;
            slider.minimumValue = -10;
            slider.maximumValue = 10;
            timer.running = false;
        }
    }
    Button {
        text: "inc value"

        property bool running : timer.running

        onClicked: {
            timer.running = true;
        }
    }
    Timer {
        id: timer
        interval: 100
        running: false
        repeat: true
        onTriggered: {
            slider.value += 1;
            running = (slider.value!==slider.maximumValue)
        }
    }
}
