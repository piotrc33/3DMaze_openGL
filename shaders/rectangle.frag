#version 330 
#extension GL_ARB_explicit_uniform_location : require
out vec4 color;

void main(void) {
  float grey = 0.7;
  color = vec4(grey, grey, grey, 1.0 );
} 