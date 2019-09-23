// Fragment shader: : Reflection shading
// ================
#version 450 core

// Ouput data
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube cubemap;

void main()
{
   vec3 I = normalize(Position - cameraPos);
   float ratio = 1.00 / 1.52;
   vec3 R = refract(I, normalize(Normal), ratio);
   FragColor = vec4(texture(cubemap, R).rgb, 1.0);
}