#version 460 core

in vec3 v_position;
in vec3 v_normal;
in vec4 v_lightSpacePos;

out vec4 v_fragColor;

struct Light {
    vec3 position;
    vec3 direction;
    vec4 color;
    float intensity;
};

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emissive;
    float shine;
};

uniform sampler2D u_shadowMap;
uniform float u_globalAmbient;
uniform vec3 u_cameraPos;
uniform Light light;
uniform Material mat;

float calculateShadowValue(vec4 fragPosLightSpace, float NoL, int sampleRadius);
float pcfFilter(sampler2D shadowMap, vec3 projCoords, float currentDepth, float closestDepth, float bias, int sampleRadius);

void main() {
    vec3 N = normalize(v_normal);
    vec3 L = normalize(-light.direction);

    vec4 ambient = light.color * mat.ambient * u_globalAmbient;

    float NoL = dot(N, L);
    float diff = max(NoL, 0.0);
    vec4 diffuse = diff * light.color * mat.diffuse;

    vec3 V = normalize(u_cameraPos - v_position);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), mat.shine);
    vec4 specular = spec * light.color * mat.specular;

    float shadow = calculateShadowValue(v_lightSpacePos, NoL, 3);
    vec4 finalColor = ambient + (diffuse + specular + mat.emissive) * (1.0 - shadow);
  
    v_fragColor = finalColor;
}

float calculateShadowValue(vec4 fragPosLightSpace, float NoL, int sampleRadius) {
    // transformed fragment position to normalized device coordinates (NDC) in the range [-1, 1]
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // depth value of the closest object in the shadow map at the same position as the fragment
    float closestDepthValue = texture(u_shadowMap, projCoords.xy).r;

    // depth value of the fragment in light space
    float currentDepthValue = projCoords.z;

    // bias value for the shadow map lookup based on the angle between the normal and the light direction
    //float bias = max(0.005 * tan(acos(NoL)), 0.001);
    float bias = max(0.0 * tan(acos(NoL)), 0.005);

    // PCF filtering to smooth out the shadow edges
    float shadowValue = pcfFilter(u_shadowMap, projCoords, currentDepthValue, closestDepthValue, bias, sampleRadius);

    // set the shadow value to 0.0 if the fragment is outside the far plane of the light's frustum
    if(projCoords.z > 1.0) {
        shadowValue = 0.0;
    }

    return shadowValue;
}

// Percentage-Closer Filtering (PCF) to smooth out the shadow edges
float pcfFilter(sampler2D shadowMap, vec3 projCoords, float currentDepth, float closestDepth, float bias, int sampleRadius) {
    // If the sample radius is 0, use a simple binary shadow test instead of PCF filtering
    if(sampleRadius == 0) {
        return currentDepth - bias > closestDepth ? 1.0 : 0.0;
    }

    // sampling the shadow map in a grid around the current fragment
    float shadowValue = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -sampleRadius; x <= sampleRadius; ++x) {
        for(int y = -sampleRadius; y <= sampleRadius; ++y) {
            float pcfDepthValue = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadowValue += currentDepth - bias > pcfDepthValue ? 1.0 : 0.0;  
        }    
    }
    shadowValue /= float((sampleRadius * 2 + 1) * (sampleRadius * 2 + 1));
    return shadowValue;
}
