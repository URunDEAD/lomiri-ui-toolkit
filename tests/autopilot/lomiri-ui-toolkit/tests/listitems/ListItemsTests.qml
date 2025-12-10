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

Item {
    width: 350
    height: 500
    Column {
        anchors.fill: parent

        ListItem.Header { 
            id: listItemHeader
            text: "Header" 
        }
        ListItem.Standard {
            id: listItemStandard
            text: "Standard"
        }
        ListItem.SingleValue {
            id: listItemSingleValue
            text: "Single Value"
            value: "Value"
        }
        ListItem.MultiValue {
	    id: listItemMultiValue
            text: "Multi Value"
            values: ["Value 1", "Value 2", "Value 3", "Value 4"]
        }

        ListItem.Subtitled {
            id: listItemSubtitled
            text: "Subtitled"
            subText: "Secondary label"
        }

        ListItem.Caption {
            id: listItemCaption
            text: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris hendrerit, erat ut vulputate dignissim, augue est vulputate ante, non tempus nulla est quis purus."
        }

        ListItem.Divider {
            id: listItemDivider
        }

        ListItem.ValueSelector {
            id: listItemValueSelector
            text: "Value Selector"
            values: ["Value 1", "Value 2", "Value 3", "Value 4"]
        }

        ListItem.Standard {
            id: listItemWithControl
            text: "With Control"
            control: Switch {
                anchors {
                   top: parent.top
               bottom: parent.bottom
                }
            }
        }
    }
}
