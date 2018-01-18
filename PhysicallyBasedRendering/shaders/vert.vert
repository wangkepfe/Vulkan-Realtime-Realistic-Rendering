#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 modelMatrix;
    mat4 mvpMatrix;
    vec3 viewPos;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBitangent;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragColor;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 tangentViewPos;
layout(location = 4) out vec3 tangentFragPos;
layout(location = 5) out vec3 tangentLightPos;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    fragPos = vec3(ubo.modelMatrix * vec4(inPosition, 1.0));  
    fragColor = inColor;
    fragTexCoord = inTexCoord;

    vec3 N = vec3(ubo.modelMatrix * vec4(inNormal, 1.0));
    vec3 T = vec3(ubo.modelMatrix * vec4(inTangent, 1.0));
    //B is calculatable!
    //T = normalize(T - dot(T, N) * N);
    //vec3 B = cross(N, T);
    vec3 B = vec3(ubo.modelMatrix * vec4(inBitangent, 1.0));
    mat3 TBN = transpose(mat3(T, B, N));

    vec3 pointLightPos = vec3(2.0, 1.0, 0.0);

    tangentViewPos = TBN * ubo.viewPos;
    tangentFragPos = TBN * fragPos;
    tangentLightPos = TBN * pointLightPos;

    gl_Position = ubo.mvpMatrix * vec4(inPosition, 1.0);
}