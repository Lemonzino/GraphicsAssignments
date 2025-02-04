#version 450 core

in vec2 _TexCoord;
// Ouput data
out vec3 fragColor;

in vec3 E;
in vec3 N;
in vec3 L;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

struct PointLight{
	vec3 position;
	vec3 color;
	float power;
 };
uniform PointLight light;

// Values that stay constant for the whole mesh.
uniform sampler2D textureBuffer;

void main(){
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
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4.0f * material.shininess);
    
    vec3 specular =  light.power * light.color * (spec * material.specular);  
    
    // specular *= diff;
    vec3 result = ambient + diffuse + specular;
	
	fragColor = result * texture(textureBuffer,_TexCoord).xyz;
}
