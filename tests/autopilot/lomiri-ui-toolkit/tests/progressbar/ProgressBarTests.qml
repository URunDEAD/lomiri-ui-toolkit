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
    ProgressBar {
       id: progressBar
       minimumValue: 1
       maximumValue: 10
    }
    Button {
        text: "indeterminate"
        onClicked: {
            progressBar.indeterminate = !progressBar.indeterminate;
        }
    }
    Button {
        text: "minmax_values"
        onClicked: {
            progressBar.minimumValue = progressBar.minimumValue + 1;
            progressBar.maximumValue = progressBar.maximumValue + 10;
        }
    }
    Button {
        text: "reset values"
        onClicked: {
            progressBar.value = -10;
            progressBar.minimumValue = -10;
            progressBar.maximumValue = 10;
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
            progressBar.value += 1;
            running = (progressBar.value!==progressBar.maximumValue)
        }
    }
}
