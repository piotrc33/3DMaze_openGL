#version 330 
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
layout(location = 0) in vec3 pos;
// layout(location = 1) uniform vec3 v1;
// layout(location = 2) uniform vec3 v2;
// layout(location = 3) uniform vec3 v3;
// layout(location = 1) uniform mat4 trans;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 projection;
out vec4 vcolor;
void main(void) {
  // const vec3 vertices[3] = vec3[3](vec3( v1.x, v1.y, v1.z),
  //                                   vec3( v2.x, v2.y, v2.z),
  //                                   vec3( v3.x, v3.y, v3.z));

  // float red = (v1.x + v2.x + v3.x)/3;
  // float red = (pos.x + 1) / 2;
  // float green = (v1.y + v2.y + v3.y)/3;
  // float green = (pos.y + 1) / 2;
  // float blue = (v1.z + v2.z + v3.z)/3;
  // float blue = (pos.z + 1) / 2;

  // vcolor      = vec4(red, green, blue, 1.0);
  gl_Position = projection * translation * rotation * vec4(pos, 1.0); 
  float red = (gl_Position.x + 1) / 2;
  float green = (gl_Position.y + 1) / 2;
  float blue = (gl_Position.z + 1) / 2;

  vcolor      = vec4(red, green, blue, 1.0);
}