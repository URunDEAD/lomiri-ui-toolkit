#version 440

layout(location = 0) in vec2 coord;

layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    float shift;
    float maximumValue;
    vec4 color;
    float threshold;
    vec4 aboveThresholdColor;
};

layout(binding = 1) uniform sampler2D texture0;

void main() {
    vec4 tex = texture(texture0, vec2(coord.x, coord.y));
    float value = tex.r * 255.0;
    float isOn = 1.0 - step(value / maximumValue, 1.0 - coord.y);
    float isAboveThreshold = step(threshold, value);

    fragColor = mix(vec4(0.0), mix(color, aboveThresholdColor, isAboveThreshold), isOn) * qt_Opacity;
}
