#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf { 
    mat4 qt_Matrix;
    float qt_Opacity;

    vec4 texCoordRange;
    float scaleFactor;
    vec4 outputColor;
} ubuf;

layout(binding = 1) uniform sampler2D source;

void main() {
    vec4 tex = texture(source, qt_TexCoord0);
    fragColor = vec4(ubuf.outputColor.rgb, ubuf.outputColor.a*tex.a) * ubuf.qt_Opacity;
}
