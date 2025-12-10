/*
 * Copyright 2014 Canonical Ltd.
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

import QtQuick 2.4
import Lomiri.Components 2.0
import Lomiri.PerformanceMetrics 1.0 as PerformanceMetrics

Item {
    id: graph
    
    property var model
    property real maximumValue
    property alias labels: labelsRepeater.model
    property alias backgroundColor: background.color
    property color color: Qt.rgba(0.4, 0.73, 0.4, 1.0)
    property real threshold: 16.0
    property color aboveThresholdColor: "#ff4e00"
    property string labelFormat: "%1"

    implicitWidth: units.gu(38)
    implicitHeight: units.gu(8)

    Rectangle {
        id: background
        anchors.fill: parent
        color: Qt.rgba(0.0, 0.0, 0.0, 0.8)
    }

    PerformanceMetrics.TextureFromImage {
        id: texture
        image: graph.model.image
    }

    ShaderEffect {
        anchors.fill: parent

        property var texture0: texture
        property real shift: graph.model.shift / graph.model.samples
        property real maximumValue: graph.maximumValue
        property color color: graph.color
        property real threshold: graph.threshold
        property color aboveThresholdColor: graph.aboveThresholdColor

        vertexShader: "qrc:/2.0/shaders/bargraph.vert.qsb"
        fragmentShader: "qrc:/2.0/shaders/bargraph.frag.qsb"
    }

    Repeater {
        id: labelsRepeater

        Item {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                bottomMargin: modelData.value / graph.maximumValue * parent.height
            }

            Label {
                id: label
                anchors {
                    left: parent.left
                    leftMargin: units.gu(0.5)
                    verticalCenter: parent.verticalCenter
                }
                text: modelData.label
                color: modelData.color
                textSize: Label.XSmall
            }

            Rectangle {
                anchors {
                    left: label.right
                    leftMargin: units.gu(0.5)
                    right: parent.right
                }
                height: 1
                color: modelData.color
            }
        }
    }

    Label {
        anchors {
            top: parent.top
            topMargin: units.gu(1)
            right: parent.right
            rightMargin: units.gu(1)
        }
        text: graph.labelFormat.arg(graph.model.currentValue)
        color: "white"
    }
}
