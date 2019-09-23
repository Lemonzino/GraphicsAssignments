// Vertex shader: Gouraud shading
// ================
#version 450 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 LightingColor; // resulting color from lighting calculations

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

    //Trasformo i vettori in coordinate vista
    // Position in VCS
	vec4 eyePosition = V * M * vec4(aPos, 1.0);
    //Moltiplico solo per V perchè la luce è fissa
	// LightPos in VCS
	vec4 eyeLightPos = V * vec4(light.position, 1.0);

	// Compute vectors E,L,N in VCS

    //Vettore di vista (va dall'osservatore al punto in questione)
	vec3 E = -eyePosition.xyz;

    //Vettore luce
	vec3 L = (eyeLightPos - eyePosition).xyz;

    //Vettore normale
	vec3 N = transpose(inverse(mat3(V * M))) * aNormal;

    // ambient
    vec3 ambient = light.power * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(N);
    vec3 lightDir = normalize(L);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.power * light.color * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(E);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  light.power * light.color * (spec * material.specular);  

    LightingColor = ambient + diffuse + specular;
}