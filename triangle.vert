#version 330 
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
layout(location = 1) uniform vec3 v1;
layout(location = 2) uniform vec3 v2;
layout(location = 3) uniform vec3 v3;
out vec4 vcolor;
void main(void) {
  const vec3 vertices[3] = vec3[3](vec3( v1.x, v1.y, v1.z),
                                    vec3( v2.x, v2.y, v2.z),
                                    vec3( v3.x, v3.y, v3.z));

  float red = (v1.x + v2.x + v3.x)/3;
  red = (red + 1) / 2;
  float green = (v1.y + v2.y + v3.y)/3;
  green = (green + 1) / 2;
  float blue = (v1.z + v2.z + v3.z)/3;
  blue = (blue + 1) / 2;
  const vec4 colors[]    = vec4[3](vec4(red, green, blue, 1.0),
                                    vec4(red, green, blue, 1.0),
                                    vec4(red, green, blue, 1.0));

  vcolor      = colors[gl_VertexID];
  gl_Position = vec4(vertices[gl_VertexID], 1.0); 
}