#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace; // Pozi?ia fragmentului în spa?iul luminii

out vec4 fColor;

// Lighting
uniform vec3 lightDir;
uniform vec3 lightColor;

// Texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap; // Harta umbrelor

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;

void computeLightComponents() {		
    vec3 cameraPosEye = vec3(0.0f); // În coordonatele camerei, observatorul este la origine

    // Transform normal
    vec3 normalEye = normalize(fNormal);	

    // Compute light direction
    vec3 lightDirN = normalize(lightDir);

    // Compute view direction 
    vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);

    // Compute ambient light
    ambient = ambientStrength * lightColor;

    // Compute diffuse light
    diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;

    // Compute specular light
    vec3 reflection = reflect(-lightDirN, normalEye);
    float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
    specular = specularStrength * specCoeff * lightColor;
}

// Functie pentru calculul umbrei
float computeShadow(vec4 fragPosLightSpace) {
    // Transform coordonate în intervalul [0,1]
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // Fragmentele în afara frustumului sunt iluminate
    if (projCoords.z > 1.0) return 0.0;

    // Adâncimea cea mai apropiat? din hart?
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // Adâncimea fragmentului curent
    float currentDepth = projCoords.z;

    // Aplic? un bias pentru a reduce "shadow acne"
    float bias = max(0.005 * (1.0 - dot(normalize(fNormal), normalize(lightDir))), 0.005);

    // Compar? adâncimile
    return currentDepth - bias > closestDepth ? 1.0 : 0.0;
}

void main() {
    computeLightComponents();
    
    // Calcularea umbrei
    float shadow = computeShadow(fragPosLightSpace);

    // Aplic? umbrele asupra culorii finale
    vec3 baseColor = vec3(0.9f, 0.35f, 0.0f); // Orange

    ambient *= texture(diffuseTexture, fTexCoords).rgb;
    diffuse *= texture(diffuseTexture, fTexCoords).rgb;
    specular *= texture(specularTexture, fTexCoords).rgb;

    // Aplic? factorul de umbr?
    vec3 color = min(ambient + (1.0 - shadow) * (diffuse + specular), 1.0f);

    fColor = vec4(color, 1.0f);
}
