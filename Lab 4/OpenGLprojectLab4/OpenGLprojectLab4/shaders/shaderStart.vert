#version 400 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColour;

//variabila culoare pe care o trimitem in fragment shader 
out vec3 colour;

uniform float uniformX;
uniform float uniformY;

void main() {
    colour = vertexColour;
    gl_Position = vec4(vertexPosition + vec3(uniformX, uniformY, 0), 1.0);
}
