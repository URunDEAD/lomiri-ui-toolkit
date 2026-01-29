#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texcoord;

layout(location = 0) out vec2 qt_TexCoord0;

layout(std140, binding = 0) uniform buf { 
    mat4 qt_Matrix;
    float qt_Opacity;

    vec4 texCoordRange;
    float scaleFactor;
    vec4 outputColor;
} ubuf;

void main() {
    vec2 texCoord = vec2(0.5 - 1.0 / (2.0 * ubuf.scaleFactor)) + texcoord / vec2(ubuf.scaleFactor);
    qt_TexCoord0 = ubuf.texCoordRange.xy + texCoord*ubuf.texCoordRange.zw;
    gl_Position = ubuf.qt_Matrix * position;
}
