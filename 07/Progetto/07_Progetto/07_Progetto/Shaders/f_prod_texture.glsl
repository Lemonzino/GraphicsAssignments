#version 450 core

in vec2 _TexCoord;
// Ouput data
out vec3 fragColor;

// Values that stay constant for the whole mesh.
uniform sampler2D textureBuffer;

void main(){

	//Modificare f_phong ponendo questo sotto come colore diffusivo
	fragColor = texture(textureBuffer,_TexCoord).xyz;
}
