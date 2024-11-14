//vertex shader for no lighting
#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;

//matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//lighting
uniform vec3 lightColor;
uniform vec3 baseColor;

out vec3 color;

void main()
{		
	//compute final vertex color
	color = baseColor;

	//transform vertex
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
}
