#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;

    vec4 leftColor;
    vec4 rightColor;
    float progress;
} ubuf;

layout(binding = 1) uniform sampler2D source;

void main() {
    vec4 sourceColor = texture(source, qt_TexCoord0);
    // TODO: BSHAH: fix mirroring
    vec4 newColor = mix(ubuf.leftColor, ubuf.rightColor, step(ubuf.progress, qt_TexCoord0.x));
    fragColor = newColor * sourceColor.a * ubuf.qt_Opacity;
}
