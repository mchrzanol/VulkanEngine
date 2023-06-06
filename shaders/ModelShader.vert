#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

struct EntityUBO {
    mat4 model;
    vec3 color;
};
layout(set = 1, binding = 0) readonly buffer modelUBO {
    EntityUBO m_UBO[10000];
};

void main() {
    gl_Position = ubo.proj * ubo.view * m_UBO[0].model *vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;

}