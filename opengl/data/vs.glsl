#version 330
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vnormal;
layout(location = 2) in vec2 texCoordinates;

out vec3 fp;
out vec3 fnormal;
out vec2 ftex;

uniform mat4 model;
uniform mat4 projection_view;

void main() {
   fp = vec3(model*vec4(vp,1.0));
   fnormal = mat3(transpose(inverse(model)))*vnormal;

   gl_Position =  projection_view * model * vec4(vp,1.0);
   ftex = texCoordinates;
}