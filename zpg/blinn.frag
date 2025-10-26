#version 330 core
#define MAX_LIGHTS 8

in vec4 worldPosition; 
in vec3 worldNormal;

struct Light {
    vec4 position;   
    vec4 ambient;
    vec4 diffuse;    
    vec4 specular;   
    float constant;   
    float linear;    
    float quadratic;  
}; 

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform int numberOfLights;
uniform Light lights[MAX_LIGHTS];

out vec4 fragColor;

void main()
{
  vec3 norm = normalize(worldNormal);
    vec3 viewDir = normalize(viewPos - worldPosition.xyz);

    vec3 totalAmbient = vec3(0.0);
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    for (int i = 0; i < numberOfLights; i++)
    {
        float dist = length(lights[i].position.xyz - worldPosition.xyz);
        float attenuation = 1.0 / (lights[i].constant + 
                               lights[i].linear * dist + 
                               lights[i].quadratic * (dist * dist));
        
        totalAmbient += lights[i].ambient.rgb * attenuation;

        vec3 lightDir = normalize(lights[i].position.xyz - worldPosition.xyz);
        float diff = max(dot(norm, lightDir), 0.0);
        totalDiffuse += diff * lights[i].diffuse.rgb * attenuation;

        float specularStrength = 0.5;
        vec3 halfwayDir = normalize(lightDir + viewDir); 
        float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0); 
        totalSpecular += specularStrength * spec * lights[i].specular.rgb * attenuation;
    }
    
    vec3 result = (totalAmbient + totalDiffuse) * objectColor + totalSpecular;
    fragColor = vec4(result, 1.0);
}