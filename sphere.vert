#version 330 
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
layout(location = 0) in vec3 pos;
uniform mat4 projection;
uniform mat4 translation;
uniform mat4 camera;

void main(void) {
  gl_Position = projection * camera * translation * vec4(pos, 1.0);
}