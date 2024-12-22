#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fragTexCoords; // Adăugat pentru coordonate de textură

out vec4 fColor;

// lighting
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 lightPosEye; // Declarație pentru poziția luminii
uniform sampler2D diffuseTexture; // Declarație pentru textura difuză
uniform sampler2D specularTexture; // Declarație pentru textura speculară

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;

void computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f); // în coordonate eye, observatorul este la origine
	
	// transform normal
	vec3 normalEye = normalize(fNormal);	
	
	// compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	// compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
	// compute ambient light
	ambient = ambientStrength * lightColor;
	
	// compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	// compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;
}

void main() 
{
	computeLightComponents();
	
	vec3 baseColor = vec3(0.9f, 0.35f, 0.0f); // orange

	float constant = 1.0f; 
	float linear = 0.0045f; 
	float quadratic = 0.0075f;

	// compute distance to light 
	float dist = length(lightPosEye - fPosEye.xyz); 
	
	// compute attenuation 
	float att = 1.0f / (constant + linear * dist + quadratic * (dist * dist)); 
	
	// Atenuarea influențează toate componentele luminii calculate anterior
	ambient *= att;
	diffuse *= att;
	specular *= att;

	// Aplicăm texturile la lumină
	ambient *= texture(diffuseTexture, fragTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fragTexCoords).rgb;
	specular *= texture(specularTexture, fragTexCoords).rgb;
	
	// Calculăm culoarea finală
	vec3 color = min((ambient + diffuse) + specular, vec3(1.0f));
    
    fColor = vec4(color, 1.0f);
}
