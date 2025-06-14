// Copyright © 2015 Canonical Ltd.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; version 3.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Author: Loïc Molinari <loic.molinari@canonical.com>

#version 440

layout(location = 0) in vec4 positionAttrib;
layout(location = 1) in vec2 shapeCoordAttrib;
layout(location = 2) in vec4 sourceCoordAttrib;
layout(location = 3) in float yCoordAttrib;
layout(location = 4) in vec4 backgroundColorAttrib;

layout(std140, binding=0) uniform buf {
    mat4 matrix;
    vec2 opacityFactors;
    float sourceOpacity;
    float distanceAA;
    // FIXME workaround for Qt <= 6.7
    // bool textured;
    int textured;
    int aspect;
} ubuf;

// FIXME(loicm) Optimize by reducing/packing varyings.
layout(location = 0) out vec2 shapeCoord;
layout(location = 1) out vec4 sourceCoord;
layout(location = 2) out float yCoord;
layout(location = 3) out vec4 backgroundColor;

void main()
{
    shapeCoord = shapeCoordAttrib;
    if (ubuf.textured != 0) {
        sourceCoord = sourceCoordAttrib;
    }
    yCoord = yCoordAttrib;
    backgroundColor = backgroundColorAttrib;

    gl_Position = ubuf.matrix * positionAttrib;
}
