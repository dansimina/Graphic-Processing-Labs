#version 410 core

uniform vec3 baseColor;
uniform vec3 lightColor;

in vec3 fragPosEye;
in vec3 normalEye;
in vec3 lightDirN;

out vec4 fColor;

float ambientStrength = 0.2f;
float specularStrength = 0.5f;
int shininess = 32;

void main()
{
    // Normalizează normalele interpolate
    vec3 norm = normalize(normalEye);

    // Calculează direcția către observator
    vec3 viewDir = normalize(-fragPosEye);

    // Iluminare ambientală
    vec3 ambient = ambientStrength * lightColor;

    // Iluminare difuză
    float diff = max(dot(norm, lightDirN), 0.0f);
    vec3 diffuse = diff * lightColor;

    // Iluminare speculară
    vec3 reflectDir = reflect(-lightDirN, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // Calculează culoarea finală
    vec3 finalColor = min((ambient + diffuse) * baseColor + specular, 1.0f);

    // Setează culoarea fragmentului
    fColor = vec4(finalColor, 1.0f);
}