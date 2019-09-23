// Fragment shader: : Gouraud shading
// ================
#version 450 core

// Ouput data
out vec4 FragColor;

in vec3 color; 

void main()
{
   FragColor = vec4(color, 1.0);
}
