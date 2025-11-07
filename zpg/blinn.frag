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
    
    vec3 direction;      
    float cutOff;        
    float outerCutOff;  
    int type;            
    int enabled;         
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
        if (lights[i].enabled == 0) {
            continue;
        }

        vec3 lightDir;
        float attenuation = 1.0;
        float intensity = 1.0;

        if (lights[i].type == 0) {
            // Point Light
            lightDir = normalize(lights[i].position.xyz - worldPosition.xyz);
            float dist = length(lights[i].position.xyz - worldPosition.xyz);
            attenuation = 1.0 / (lights[i].constant + 
                               lights[i].linear * dist + 
                               lights[i].quadratic * (dist * dist));
        }
        else if (lights[i].type == 1) {
            // Directional Light
            lightDir = normalize(-lights[i].direction);
            attenuation = 1.0;
        }
        else if (lights[i].type == 2) {
            // Spot Light
            lightDir = normalize(lights[i].position.xyz - worldPosition.xyz);
            float dist = length(lights[i].position.xyz - worldPosition.xyz);
            attenuation = 1.0 / (lights[i].constant + 
                               lights[i].linear * dist + 
                               lights[i].quadratic * (dist * dist));
            
            float theta = dot(lightDir, normalize(-lights[i].direction));
            float epsilon = lights[i].cutOff - lights[i].outerCutOff;
            intensity = clamp((theta - lights[i].outerCutOff) / epsilon, 0.0, 1.0);
        }

        totalAmbient += lights[i].ambient.rgb * attenuation;

        float diff = max(dot(norm, lightDir), 0.0);
        totalDiffuse += diff * lights[i].diffuse.rgb * attenuation * intensity;

        // Blinn-Phong specular
        float specularStrength = 0.5;
        vec3 halfwayDir = normalize(lightDir + viewDir); 
        float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0); 
        totalSpecular += specularStrength * spec * lights[i].specular.rgb * attenuation * intensity;
    }
    
    vec3 result = (totalAmbient + totalDiffuse) * objectColor + totalSpecular;
    fragColor = vec4(result, 1.0);
}