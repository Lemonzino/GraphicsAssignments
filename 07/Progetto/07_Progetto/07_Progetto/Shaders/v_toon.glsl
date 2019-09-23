// Vertex shader: Gouraud shading
// ================
#version 450 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 color; // resulting color from lighting calculations

// Values that stay constant for the whole mesh.
uniform mat4 P; //matrice di proiezione
uniform mat4 V; //matrice di vista
uniform mat4 M; //matrice di modelling
// position*rotation*scaling

struct PointLight{
	vec3 position;
	vec3 color;
	float power;
 };
uniform PointLight light;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

void main()
{
    gl_Position = P * V * M * vec4(aPos, 1.0);

//Calcolo la mia posizione
	vec4 eyePosition = V * M * vec4(aPos, 1.0);
//Calcolo la posizione della luce
    vec4 eyeLightPos = V * vec4(light.position, 1.0);
//Calcolo la direzione della luce (sottraggo la posizione della luce con la mia posizione)
    vec3 L = (eyeLightPos - eyePosition).xyz;
//Normalizzo il vettore
    vec3 lightDir = normalize(L);


    //Vettore normale
	vec3 N = transpose(inverse(mat3(V * M))) * aNormal;
    vec3 norm = normalize(N);

    float dotValue = dot(norm, lightDir);
    if(dotValue < 0.15) {
        color = vec3(0.1f, 0.1f, 0.1f);
    } else if(dotValue < 0.85) {
        color = vec3(0.6f, 0.6f, 0.6f);
    } else {
        color = vec3(1.0f, 1.0f, 1.0f);
    }
}