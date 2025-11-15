#version 330 core

in vec2 fragUV;

uniform sampler2D textureUnitID;

out vec4 fragColor;

void main() {
    
    fragColor = texture(textureUnitID, fragUV);
}