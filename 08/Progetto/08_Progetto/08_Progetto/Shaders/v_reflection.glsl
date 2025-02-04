#version 450 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

// Values that stay constant for the whole mesh.
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 cameraPos;

void main()
{
    Normal = mat3(transpose(inverse(M))) * aNormal;
    Position = vec3(M * vec4(aPos, 1.0));
    gl_Position = P * V * M * vec4(aPos, 1.0);
}