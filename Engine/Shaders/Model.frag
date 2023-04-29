#version 460 core

in vec3 v_position;
in vec3 v_normal;
out vec4 v_fragColor;

struct StarLight {
    vec4 color;
    vec3 direction;
};

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shine;
};

uniform float u_globalAmbient;
uniform vec3 u_cameraPos;
uniform StarLight light;
uniform Material mat;

void main() {

    vec3 N = normalize(v_normal);
    vec3 L = normalize(-light.direction);

    //vec4 ambient = (light.color * mat.ambient) + (u_globalAmbient * mat.ambient);
    vec4 ambient = light.color * mat.ambient * u_globalAmbient;

    float diff = max(dot(N, L), 0.0);
    vec4 diffuse = diff * light.color * mat.diffuse;

    vec3 V = normalize(u_cameraPos - v_position);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), mat.shine);
    vec4 specular = spec * light.color * mat.specular;

    vec4 finalColor = ambient + diffuse + specular; 
    v_fragColor = vec4(finalColor.rgb, 1.0);
}

