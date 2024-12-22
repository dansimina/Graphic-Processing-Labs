#version 400

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColour;
layout(location = 2) in vec3 vertexNormal;

out vec3 colour;
out vec3 normal;

void main() {
    colour = vertexColour;
    normal = vertexNormal;
    gl_Position = vec4(vertexPosition, 1.0);
}
