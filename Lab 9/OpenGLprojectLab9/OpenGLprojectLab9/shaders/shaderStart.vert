#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fNormal;
out vec4 fPosEye;
out vec2 fTexCoords;
out vec4 fragPosLightSpace; // Pozi?ia fragmentului în spa?iul luminii

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 lightSpaceTrMatrix; // Matricea de transformare în spa?iul luminii

void main() 
{
    // Compute eye space coordinates
    fPosEye = view * model * vec4(vPosition, 1.0f);
    fNormal = normalize(normalMatrix * vNormal);
    fTexCoords = vTexCoords;

    // Transform? pozi?ia fragmentului în spa?iul luminii
    fragPosLightSpace = lightSpaceTrMatrix * model * vec4(vPosition, 1.0f);

    // Compute final position
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);
}
