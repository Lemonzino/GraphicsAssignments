#version 450 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;

out vec3 _3D_TexCoord;

// Values that stay constant for the whole mesh.
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 camera_position;


void main()
{
    _3D_TexCoord = aPos; 

    gl_Position = P * V * M * vec4(aPos, 1.0);
}  