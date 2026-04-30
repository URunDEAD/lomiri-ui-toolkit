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
    width: units.gu(50)
    height: units.gu(80)

    id: root
    property real listViewHeaderHeight: units.gu(5)

    MainView {
        id: mainViewHeader
        anchors.fill: parent

        Page {
            id: page
            title: "listview"

            ListView {
                anchors.fill: parent
                id: listView

                header: Rectangle {
                    color: "blue"
                    width: parent.width
                    height: root.listViewHeaderHeight
                }
                model: 500
                delegate: Label {
                    text: "number " +index
                }
            }
        }

        Item {
            // Wrapping the Page inside this Item should not
            // affect the header alignment, see bug #1261907.
            anchors.fill: parent
            id: wrappingItem

            Page {
                id: wrappedPage
                title: "wrapped listview"

                ListView {
                    anchors.fill: parent
                    id: wrappedListView

                    header: Rectangle {
                        color: "red"
                        width: parent.width
                        height: root.listViewHeaderHeight
                    }
                    model: 500
                    delegate: Label {
                        text: "number " +index
                    }
                }
            }
        }
    }

    TestCase {
        name: "HeaderAlignment"
        when: windowShown

        function init() {
            // Make sure that not both pages are visible simultaneously.
            // Only the active page will have the header spacing applied.
            page.active = false;
            page.visible = false;
            wrappedPage.active = false;
            wrappingItem.visible = false;
        }

        function test_ListViewHeaderAlignment_bug1202277() {
            page.visible = true;
            page.active = true;
            waitForRendering(mainViewHeader);
            compare(listView.contentY, -root.listViewHeaderHeight - mainViewHeader.__propagated.header.height,
                    "ListView header is aligned with the MainView header");
        }

        function test_WrappedListViewHeaderAlignment_bug1261907() {
            wrappingItem.visible = true;
            wrappedPage.active = true;
            waitForRendering(mainViewHeader);
            compare(wrappedListView.contentY, -root.listViewHeaderHeight - mainViewHeader.__propagated.header.height,
                    "ListView header inside wrapped Page is aligned with the MainView header");
        }
    }
}
