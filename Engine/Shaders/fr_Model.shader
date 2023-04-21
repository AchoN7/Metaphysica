#version 460 core

in vec3 v_position;
in vec3 v_normal;

uniform vec3 u_lightDirection;
uniform vec3 u_lightColor;
uniform vec3 u_cameraPosition;
uniform vec3 u_modelColor;

out vec4 FragColor;

void main() {

    vec3 ambient = 0.1 * u_lightColor;

    vec3 norm = normalize(v_normal);
    //vec3 lightDir = normalize(u_lightPosition - v_position);
    vec3 lightDir = normalize(-u_lightDirection);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    float specStrength = 0.0;
    vec3 viewDir = normalize(u_cameraPosition - v_position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specStrength * spec * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_modelColor;
    FragColor = vec4(result, 1.0);
}

