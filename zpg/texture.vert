#version 330 core
layout(location = 0) in vec3 aPos;    
layout(location = 1) in vec3 aNormal; 
layout(location = 2) in vec2 aUV;     

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec4 worldPosition; 
out vec3 worldNormal;
out vec2 fragUV;  

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
    
    worldPosition = modelMatrix * vec4(aPos, 1.0f);
    worldNormal = normalize(transpose(inverse(mat3(modelMatrix))) * aNormal);
    fragUV = aUV * 10.0;  
}