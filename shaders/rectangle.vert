#version 330 
#extension GL_ARB_explicit_uniform_location : require
layout(location = 0) in vec3 vPos;

uniform mat4 projection;
uniform mat4 camera;

void main(void) {
    gl_Position = projection * camera * vec4(vPos, 1.0); 
  }