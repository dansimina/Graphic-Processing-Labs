#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;

//matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

//lighting
uniform vec3 lightDir;

out vec3 fragPosEye;
out vec3 normalEye;
out vec3 lightDirN;

void main()
{	
    // Transformăm poziția în coordonate Eye Space
    vec4 vertPosEye = view * model * vec4(vPosition, 1.0f);
    fragPosEye = vertPosEye.xyz;

    // Transformăm normalele în Eye Space
    normalEye = normalize(normalMatrix * vNormal);

    // Normalizează direcția luminii
    lightDirN = normalize(lightDir);

    // Poziția finală a vertexului în clip space
    gl_Position = projection * vertPosEye;
}