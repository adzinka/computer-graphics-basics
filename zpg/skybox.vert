#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix; 

void main()
{
    texCoords = aPos;
    
    mat4 view = mat4(mat3(viewMatrix)); 
    
    vec4 pos = projectionMatrix * view * vec4(aPos, 1.0);
    
    gl_Position = pos.xyww;
}