#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out int textureIndex;

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

layout(set = 1, binding = 3) readonly buffer textureIndexing {
    int textureIndexArray[10000];
};

void main() {
    gl_Position = ubo.proj * ubo.view * m_UBO[gl_InstanceIndex].model *vec4(inPosition, 1.0);
    fragColor = m_UBO[gl_InstanceIndex].color;
    fragTexCoord = inTexCoord;

    textureIndex = textureIndexArray[gl_InstanceIndex];

}