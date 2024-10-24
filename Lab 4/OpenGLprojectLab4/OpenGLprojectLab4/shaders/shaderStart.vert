#version 400
 
layout(location = 0) in vec3 vertexPosition; 
layout(location = 1) in vec3 vertexColour; 
 
out vec3 colour; 
 
uniform float offsetX;
uniform float offsetY;

void main() { 
    vec3 pos = vertexPosition;
    pos.x += offsetX;
    pos.y += offsetY;
    colour = vertexColour; 
    gl_Position = vec4(pos, 1.0); 
} 
