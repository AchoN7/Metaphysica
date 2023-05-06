#version 460 core

vec3 v_position;

uniform float u_cycle; //dot product of star position with its "noon" position

const vec3 col = vec3(0.509, 0.799, 0.9);

void main() {

	const float dayFactor = u_cycle + 0.45;

	if (dayFactor > 1.0)
		gl_FragColor = vec4(col, 1.0);
	else 
		gl_FragColor = vec4(col * dayFactor, 1.0);
		
}