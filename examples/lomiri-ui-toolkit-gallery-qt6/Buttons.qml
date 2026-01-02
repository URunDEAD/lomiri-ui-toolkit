/*
 * Copyright 2013 Canonical Ltd.
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
import Lomiri.Components 2.0
import Lomiri.Components.ListItems 2.0

Template {
    objectName: "buttonsTemplate"

    TemplateSection {
        className: "Button"

        TemplateFlow {
            title: i18n.tr("Standard")

            Button {
                objectName: "button_text"
                text: i18n.tr("Call")
            }
        }

        TemplateFlow {
            title: i18n.tr("Stroke")

            Button {
                objectName: "button_stroke"
                text: i18n.tr("Call")
                strokeColor: LomiriColors.warmGrey
            }
        }

        TemplateFlow {
            title: i18n.tr("Color")

            Button {
                objectName: "button_color"
                width: units.gu(20)
                action: Action {
                    text: i18n.tr("C&all %1").arg(shortcut)
                    shortcut: 'Ctrl+L'
                    property bool flipped
                    onTriggered: flipped = !flipped
                }
                color: action.flipped ? LomiriColors.blue : LomiriColors.green
            }
        }

        TemplateFlow {
            title: i18n.tr("Icon")

            Button {
                objectName: "button_iconsource"
                iconSource: "image://theme/call-start"
            }
        }

        TemplateFlow {
            title: i18n.tr("Icon+Text")

            Button {
                objectName: "button_iconsource_right_text"
                width: units.gu(10)
                text: i18n.tr("Call")
                iconSource: "image://theme/call-start"
                iconPosition: "right"
            }

            Button {
                objectName: "button_iconsource_left_text"
                width: units.gu(10)
                text: i18n.tr("Call")
                iconSource: "image://theme/call-start"
            }
        }

        TemplateFlow {
            title: i18n.tr("Disabled")

            Button {
                objectName: "button_text_disabled"
                text: i18n.tr("Call")
                enabled: false
            }
        }
    }

    TemplateSection {
        className: "ComboButton"

        TemplateFlow {
            title: i18n.tr("Collapsed")

            ComboButton {
                text: "Press me"
                objectName: "combobutton_collapsed"
                width: parent.width < units.gu(30)? parent.width : units.gu(30)
                comboList:  LomiriListView {
                    model: 10
                    delegate: ListItemWithLabel {
                        title.text: "item #" + modelData
                    }
                }
            }
        }

        TemplateFlow {
            title: i18n.tr("Icon")

            ComboButton {
                iconSource: "image://theme/call-start"
                width: parent.width < units.gu(30)? parent.width : units.gu(30)
                comboList:  LomiriListView {
                    model: 10
                    delegate: ListItemWithLabel {
                        title.text: "item #" + modelData
                    }
                }
            }
        }

        TemplateFlow {
            title: i18n.tr("Icon+Text")

            ComboButton {
                text: "Answer"
                iconSource: "image://theme/call-start"
                width: parent.width < units.gu(30)? parent.width : units.gu(30)
                comboList:  LomiriListView {
                    model: 10
                    delegate: ListItemWithLabel {
                        title.text: "item #" + modelData
                    }
                }
            }
        }
        TemplateFlow {
            title: i18n.tr("Expanded")

            ComboButton {
                text: "Press me"
                objectName: "combobutton_expanded"
                expanded: true
                width: parent.width < units.gu(30)? parent.width : units.gu(30)
                comboList:  LomiriListView {
                    model: 10
                    delegate: ListItemWithLabel {
                        title.text: "item #" + modelData
                    }
                }
            }
        }
    }
}
