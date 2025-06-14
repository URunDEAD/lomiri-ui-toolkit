/*
 * Copyright 2016 Canonical Ltd.
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
 *
 * Author: Loïc Molinari <loic.molinari@canonical.com>
 */

#version 440

layout(location = 0) in vec2 outerCoord;
layout(location = 1) in vec2 innerCoord;
layout(location = 2) in vec4 color;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
} ubuf;

layout(binding = 1) uniform sampler2D sourceTexture;

void main(void)
{
    float shapeOut = texture(sourceTexture, outerCoord).r;
    float shapeIn = texture(sourceTexture, innerCoord).r;
    // Fused multiply-add friendly version of (shapeOut * (1.0 - shapeIn))
    float shape = (shapeOut * -shapeIn) + shapeOut;
    // shape is squared to make thinner corners (particularly visible at stroke 1).
    fragColor = vec4(shape * shape * ubuf.qt_Opacity) * color;
}
