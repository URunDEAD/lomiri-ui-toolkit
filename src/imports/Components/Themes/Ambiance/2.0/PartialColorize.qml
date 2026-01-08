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

import QtQuick 2.4

ShaderEffect {
    id: partialColorize

    implicitWidth: source.implicitWidth
    implicitHeight: source.implicitHeight
    visible: source !== null && source.visible

    property Item sourceItem
    property var source: ShaderEffectSource {
        hideSource: true
        sourceItem: partialColorize.sourceItem
        visible: sourceItem != null
    }
    property color leftColor
    property color rightColor
    property real progress
    property bool mirror: false
    property string texCoord: mirror ? "1.0 - qt_TexCoord0.x" : "qt_TexCoord0.x"

    fragmentShader: "qrc:/Themes/Ambiance/shaders/partialcolorize.frag.qsb"
}
