/*
 * Copyright 2015 Canonical Ltd.
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

import QtQuick 2.12
import Lomiri.Components 1.3
import Lomiri.Components.Themes 1.3

Page {
    id: aboutPage
    theme: ThemeSettings {
        name: "Lomiri.Components.Themes.SuruDark"
        palette: Palette {
            normal.background: LomiriColors.slate
        }
    }
    style: Rectangle {
        anchors.fill: parent
        color: theme.palette.normal.background
    }
    header: PageHeader {
        title: i18n.tr("About...")
    }


    Column {
        id: top
        width: parent.width
        anchors.top: header.bottom
        spacing: units.gu(2)
        padding: units.gu(2)

        Label {
            width: parent.width
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            textSize: Label.Large
            text: "Lomiri UI ToolKit" + " " + i18n.tr("gallery")
        }

        WebLink {
            label: i18n.tr("Source code")
            anchors.horizontalCenter: parent.horizontalCenter
            url: "https://gitlab.com/ubports/development/core/lomiri-ui-toolkit/-/tree/main/examples/lomiri-ui-toolkit-gallery"
            textSize: Label.Large
        }
        WebLink {
            label: i18n.tr("Design guidelines")
            anchors.horizontalCenter: parent.horizontalCenter
            url: "https://docs.ubports.com/en/latest/humanguide/index.html"
            textSize: Label.Large
        }
    }

    Column {
        spacing: units.gu(0.7)
        width: parent.width
        padding: units.gu(2)
        //anchors.top: top.bottom
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        Repeater {
            model: ListModel {
                ListElement {
                    text: "Copyright 2012-2015 Canonical Ltd."
                }
                ListElement {
                    text: "Copyright 2023 Ubports Foundation.\n\n"
                }
                ListElement {
                    text: "Contributors\n"
                }
                ListElement {
                    text: "Ubports <dev@ubports.com>"
                }
                ListElement {
                    text: "Tim Peeters (#t1mp) <tim.peeters@canonical.com>"
                }
                ListElement {
                    text: "Christian Dywan (#kalikiana) <christian.dywan@canonical.com>"
                }
                ListElement {
                    text: "Zsombor Egri (#zsombi) <zsombor.egri@canonical.com>"
                }
                ListElement {
                    text: "Florian Boucault (#Kaleo) <florian.boucault@canonical.com>"
                }
                ListElement {
                    text: "Loic Molinari (#loicm) <loic.molinari@canonical.com>"
                }
                ListElement {
                    text: "Zoltan Balogh (#bzoltan) <zoltan.balogh@canonical.com>"
                }
                ListElement {
                    text: "Benjamin Zeller (#zbenjamin) <benjamin.zeller@canonical.com>"
                }
            }
            Label {
                width: parent.width
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
                text: model.text
            }
        }
    }
}
