#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D tex[2];

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragColor;
layout(location = 3) in vec2 fragTexCoord;
layout(location = 4) in vec3 fragTangent;
layout(location = 5) in vec3 fragBitangent;
layout(location = 6) in vec3 viewPos;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 pointLightPos = vec3(2.0, 1.0, 0.0);
    vec3 lightDir = normalize(pointLightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 normalMapping = normalize(texture(tex[1], fragTexCoord).rgb * 2.0 - 1.0);
    vec3 mappingNormal = normalMapping.r * fragTangent + normalMapping.g * fragBitangent + normalMapping.b * fragNormal;
	vec3 normal = normalize(mix(fragNormal, mappingNormal, 0.995));

    //vec3 halfwayDir = normalize(lightDir + viewDir);
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0) * 0.5;
    float spec = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), 4) * 0.2;
    float diff = max(dot(normal, lightDir), 0.0);
    float ambi = 0.2;

    outColor = vec4(vec3(ambi + diff), 1.0) * texture(tex[0], fragTexCoord) + vec4(vec3(spec), 1.0);
    //outColor = vec4(fragBitangent, 1.0);
}