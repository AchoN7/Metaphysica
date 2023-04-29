#version 460 core

uniform vec4 u_starColor;

out vec4 v_fragColor;

void main()
{
    v_fragColor = u_starColor;
}