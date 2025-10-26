#version 330 core
#define MAX_LIGHTS 8

in vec4 worldPosition; 
in vec3 worldNormal;

struct Light {
    vec4 position;   
    vec4 ambient;
    vec4 diffuse;
};

uniform vec3 objectColor;
uniform int numberOfLights;
uniform Light lights[MAX_LIGHTS];

out vec4 fragColor;

void main()
{
    vec3 norm = normalize(worldNormal);
    vec3 totalLighting = vec3(0.0); 

    for (int i = 0; i < numberOfLights; i++)
    {
        vec3 ambient = lights[i].ambient.rgb;

        vec3 lightDir = normalize(lights[i].position.xyz - worldPosition.xyz);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].diffuse.rgb;

        totalLighting += (ambient + diffuse);
    }

    vec3 result = totalLighting * objectColor;
    fragColor = vec4(result, 1.0);
}