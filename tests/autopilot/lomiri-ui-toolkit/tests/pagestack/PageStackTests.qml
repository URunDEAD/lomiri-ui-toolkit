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
import Lomiri.Components.ListItems 0.1 as ListItem

PageStack {
    id: pageStack

    width: 300
    height: 400

    Component.onCompleted: push(page0)

    Page {
        id: page0
        title: "Root page"

        Column {
            anchors.fill: parent
            ListItem.Standard {
                text: "Page one"
                onClicked: pageStack.push(page1)
                progression: true
            }
        }
    }

    Page {
        id: page1
        title: "First page"

        Column {
            anchors.fill: parent
          
            ListItem.Standard {
                text: "Red rectangle"
                onClicked: pageStack.push(rect, {color: "red"})
                progression: true
            }
        }
    }

    Component {
        id: rect
        Rectangle {
            id: rectangle
            anchors.fill: parent
            color: "green"
        }
    }
}
