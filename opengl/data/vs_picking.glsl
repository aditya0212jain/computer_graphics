#version 330
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vnormal;
layout(location = 2) in vec2 texCoordinates;

uniform mat4 model;
uniform mat4 projection_view;

void main() {

   gl_Position =  projection_view * model * vec4(vp,1.0);
}