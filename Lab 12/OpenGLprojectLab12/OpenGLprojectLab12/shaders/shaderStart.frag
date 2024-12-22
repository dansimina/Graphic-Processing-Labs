#version 400

in vec3 colour;
in vec3 normal;

out vec4 fragmentColour;

void main() {
    fragmentColour.rgb = normal;
    fragmentColour.a = 1.0;
}
