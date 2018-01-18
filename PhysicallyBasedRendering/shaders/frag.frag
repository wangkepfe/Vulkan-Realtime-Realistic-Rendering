#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D tex[3];

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragColor;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 tangentViewPos;
layout(location = 4) in vec3 tangentFragPos;
layout(location = 5) in vec3 tangentLightPos;

layout(location = 0) out vec4 outColor;

vec2 parallaxMapping()
{
    const float height_scale = 0.01;
    // number of depth layers
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 parallax = viewDir.xy * height_scale; 
    vec2 deltaTexCoords = parallax / numLayers;
    // get initial values
    vec2  currentTexCoords = fragTexCoord;
    float currentDepthMapValue = texture(tex[2], currentTexCoords).r;
    
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(tex[2], currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(tex[2], prevTexCoords).r - currentLayerDepth + layerDepth;
    
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main() {
    vec3 lightDir = normalize(tangentLightPos - tangentFragPos);
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);

    // parallax mapping
    vec2 texCoords = parallaxMapping();

    // normal mapping
    vec3 normal = normalize(texture(tex[1], texCoords).rgb * 2.0 - 1.0);

    // blinn-phone
    //vec3 halfwayDir = normalize(lightDir + viewDir);
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0) * 0.5;

    float ambi = 0.2;
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), 4) * 0.2;
    
    outColor = vec4(vec3(ambi + diff), 1.0) * texture(tex[0], texCoords) + vec4(vec3(spec), 1.0);

    // debug zone
    //outColor = vec4(tangentLightPos, 1.0);
    //outColor = texture(, texCoords);
}