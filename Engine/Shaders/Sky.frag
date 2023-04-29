#version 460 core

vec3 v_position;

uniform float u_cycle;

const vec3 col = vec3(0.529, 0.811, 0.922);

void main() {

	gl_FragColor = vec4(col * u_cycle, 1.0);
		
}