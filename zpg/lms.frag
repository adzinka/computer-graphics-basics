#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 out_Color;

void main()
{
    vec3 lightPosition = vec3(10.0, 10.0, 10.0); 
    vec4 lightColor = vec4(1, 1, 1, 1.0); 
    vec4 ambientColor = vec4(0.1, 0.1, 0.1, 1.0);    

    // Vektor z tohoto fragmentu směrem ke světlu
    vec3 lightDir = normalize(lightPosition - worldPosition);
    
    // Vektor normály povrchu (musí být normalizovaný)
    vec3 normal = normalize(worldNormal);

    // Výpočet úhlu mezi normálou a směrem světla pomocí skalárního součinu
    // max(..., 0.0) zajistí, že povrchy odvrácené od světla nebudou osvětleny
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    
    // Výsledná difuzní barva = barva světla * intenzita osvětlení
    vec4 diffuse = diffuseStrength * lightColor;

    // Výsledná barva je součtem ambientní a difuzní složky
    out_Color = ambientColor + diffuse;
}