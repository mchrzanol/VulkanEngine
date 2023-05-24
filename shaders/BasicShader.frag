#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in flat int  textureIndex;

layout(set = 1, binding = 1) uniform texture2D textures[];

layout(set = 1, binding = 2) uniform sampler samplerDescriptor;

layout(location = 0) out vec4 outColor;

void main() {
   if(textureIndex == -1) {
        outColor = vec4(fragColor, 1.0);
    }
    else {
         outColor = texture(sampler2D(textures[textureIndex],  samplerDescriptor), fragTexCoord);
         }
}