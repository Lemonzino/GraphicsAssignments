// Fragment shader: Blinn shading
// ================
#version 450 core

in vec3 E;
in vec3 N;
in vec3 L;
in vec3 H;

// Ouput data
out vec3 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
struct PointLight{
	vec3 position;
	vec3 color;
	float power;
 };

uniform Material material;
uniform PointLight light;

void main()
{    
    vec3 norm = normalize(N);
	vec3 lightDir = normalize(L);    

    float dotValue = dot(norm, lightDir);
    if(dotValue < 0.15) {
        color = vec3(0.1f, 0.1f, 0.1f);
    } else if(dotValue < 0.85) {
        color = vec3(0.6f, 0.6f, 0.6f);
    } else {
        color = vec3(1.0f, 1.0f, 1.0f);
    }
}
