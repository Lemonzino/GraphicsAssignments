#version 450 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 TexCoord;

// Values that stay constant for the whole mesh.
uniform mat4 P;
uniform mat4 V;
uniform mat4 M; // position*rotation*scaling

uniform float a = 0;
uniform float w = 0;
uniform float t = 0;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	if(t == 0) {
		gl_Position =  P * V * M * vec4(vertex,1);
	} else {
		gl_Position = P * V * M * vec4(vertex.x, (a * sin(w*t + 10*vertex.x) * sin(w*t + 10*vertex.z)), vertex.z, 1);
	}

}
