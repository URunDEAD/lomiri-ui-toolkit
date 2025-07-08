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
    id: magnifier

    property ShaderEffectSource source
    property real scaleFactor: 1.2

    // Everything in the sourceItem that is not transparent will be made this color
    // in the output, but the transparency of the input is respected.
    property color outputColor: "red"

    // Specify the region of the sourceRect that must be enlarged as
    // x, y, width, height in texture coordinates. (0, 0, 1, 1) is full sourceRect.
    property rect texCoordRange: Qt.rect(0.0, 0.0, 1.0, 1.0);

    vertexShader: "qrc:/Themes/Ambiance/shaders/magnifier.vert.qsb"
    fragmentShader: "qrc:/Themes/Ambiance/shaders/magnifier.frag.qsb"
}
