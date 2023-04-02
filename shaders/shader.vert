#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(set = 1, binding = 0) uniform modelMatrix {
    mat4 model;   
}ubo_model;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo_model.model *vec4(inPosition, 1.0);
    fragColor = inColor;
}