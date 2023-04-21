#version 460 core

uniform vec3 u_starColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(u_starColor, 1.0);
}