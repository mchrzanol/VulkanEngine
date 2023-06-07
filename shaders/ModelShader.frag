#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec2 fragTexCoord;

layout(set = 1, binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {
     outColor = texture(texSampler, fragTexCoord);
}