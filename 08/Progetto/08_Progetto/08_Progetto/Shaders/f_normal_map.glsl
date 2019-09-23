// Fragment shader: : Normal mapping shading
// ================
#version 450 core

in vec2 _TexCoord;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

// Ouput data
out vec4 FragColor;

//uniforms
//Texture per il colore (diffuse) e normali (normal)
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

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
    float power = 30;
    vec3 normal = texture(normalMap, _TexCoord).rgb;
    vec3 norm = normalize(normal * 2.0 - 1.0);

    // ambient
    vec3 ambient = power * material.ambient;
	
    // diffuse 
    vec3 color = texture(diffuseMap, _TexCoord).rgb;

    //Posizione della luce - posizione del fragment
    //Ottengo il vettore della luce rispetto al fragment
	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = power * light.color * (diff * material.diffuse);
    
    // specular
	vec3 viewDir = normalize(TangentViewPos);

    vec3 reflectDir = reflect(-lightDir, norm); 
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4.0f * material.shininess);
    
    vec3 specular =  power * light.color * (spec * material.specular);  
    
    // specular *= diff;
    vec3 result = ambient + diffuse + specular;
    //FragColor = vec4(result, 1.0);
	FragColor = vec4(result * color.xyz, 1.0);
}