#version 440

#extension GL_OES_standard_derivatives : enable  // Enable dFdy() on OpenGL ES 2.

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

// Static flow control (branching on a uniform value) is fast on most GPUs (including ultra-low
// power ones) because it allows one to use the same shader execution path for an entire draw call. We
// rely on that technique here (also known as "uber-shader" solution) to avoid the complexity of
// dealing with a multiple shaders solution.
// FIXME(loicm) Validate GPU behavior with regards to static flow control.

layout(location = 0) in vec2 shapeCoord;
layout(location = 1) in vec4 sourceCoord;
layout(location = 2) in float yCoord;
layout(location = 3) in vec4 backgroundColor;
layout(location = 4) in vec2 overlayCoord;
layout(location = 5) in vec4 overlayColor;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 matrix;
    vec2 opacityFactors;
    float sourceOpacity;
    float distanceAA;
    // FIXME workaround for Qt <= 6.7
    // bool textured;
    int textured;
    int aspect;
} ubuf;

layout(binding = 1) uniform sampler2D shapeTexture;
layout(binding = 2) uniform sampler2D sourceTexture;

const int FLAT        = 0x08;  // 1 << 3
const int INSET       = 0x10;  // 1 << 4
const int DROP_SHADOW = 0x20;  // 1 << 5

void main(void)
{
    vec4 shapeData = texture(shapeTexture, shapeCoord);
    vec4 color = backgroundColor;

    if (ubuf.textured != 0) {
        // Blend the source over the current color.
        // FIXME(loicm) sign() is far from optimal. Call texture2D() at beginning of scope.
        vec2 axisMask = -sign((sourceCoord.zw * sourceCoord.zw) - vec2(1.0));
        float mask = clamp(axisMask.x + axisMask.y, 0.0, 1.0);
        vec4 source = texture(sourceTexture, sourceCoord.st) * vec4(ubuf.sourceOpacity * mask);
        color = vec4(1.0 - source.a) * color + source;
    }

    // Blend the overlay over the current color.
    // FIXME(loicm) sign() is far from optimal.
    vec2 overlayAxisMask = -sign((overlayCoord * overlayCoord) - vec2(1.0));
    float overlayMask = clamp(overlayAxisMask.x + overlayAxisMask.y, 0.0, 1.0);
    vec4 overlay = overlayColor * vec4(overlayMask);
    color = vec4(1.0 - overlay.a) * color + overlay;

    // Get the normalized distance between two pixels using screen-space derivatives of shape
    // texture coordinate. dFd*() functions have to be called outside of branches in order to work
    // correctly with VMware's "Gallium 0.4 on SVGA3D".
    float dist = length(vec2(dFdx(shapeCoord.s), dFdy(shapeCoord.s)));

    if (ubuf.aspect == FLAT) {
        // Mask the current color with an anti-aliased and resolution independent shape mask built
        // from distance fields.
        float distanceMin = abs(dist) * -ubuf.distanceAA + 0.5;
        float distanceMax = abs(dist) * ubuf.distanceAA + 0.5;
        color *= smoothstep(distanceMin, distanceMax, shapeData.b);

    } else if (ubuf.aspect == INSET) {
        // The vertex layout of the shape is made so that the derivative is negative from top to
        // middle and positive from middle to bottom.
        float shapeSide = yCoord <= 0.0 ? 0.0 : 1.0;
        // Blend the shape inner shadow over the current color. The shadow color is black, its
        // translucency is stored in the texture.
        float shadow = shapeData[int(shapeSide)];
        color = vec4(1.0 - shadow) * color + vec4(0.0, 0.0, 0.0, shadow);
        // Get the anti-aliased and resolution independent shape mask using distance fields.
        float distanceMin = abs(dist) * -ubuf.distanceAA + 0.5;
        float distanceMax = abs(dist) * ubuf.distanceAA + 0.5;
        vec2 mask = smoothstep(distanceMin, distanceMax, shapeData.ba);
        // Get the bevel color. The bevel is made of the top mask masked with the bottom mask. A
        // gradient from the bottom (1) to the middle (0) of the shape is used to factor out values
        // resulting from the mask anti-aliasing. The bevel color is white with 60% opacity.
        float bevel = (mask.x * -mask.y) + mask.x;  // -ab + a = a(1 - b)
        float gradient = clamp((shapeSide * -shapeCoord.t) + shapeSide, 0.0, 1.0);
        bevel *= gradient * 0.6;
        // Mask the current color then blend the bevel over the resulting color. We simply use
        // additive blending since the bevel has already been masked.
        color = (color * vec4(mask[int(shapeSide)])) + vec4(bevel);

    } else if (ubuf.aspect == DROP_SHADOW) {
        // Get the anti-aliased and resolution independent shape mask using distance fields.
        float distanceMin = abs(dist) * -ubuf.distanceAA + 0.5;
        float distanceMax = abs(dist) * ubuf.distanceAA + 0.5;
        int shapeSide = yCoord <= 0.0 ? 0 : 1;
        float mask = smoothstep(distanceMin, distanceMax, shapeData[shapeSide]);
        // Get the shadow color outside of the shape mask.
        float shadow = (shapeData.b * -mask) + shapeData.b;  // -ab + a = a(1 - b)
        // Mask the current color then blend the shadow over the resulting color. We simply use
        // additive blending since the shadow has already been masked.
        color = (color * vec4(mask)) + vec4(0.0, 0.0, 0.0, shadow);
    }

    fragColor = color * ubuf.opacityFactors.xxxy;
}
