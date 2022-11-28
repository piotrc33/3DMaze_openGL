#version 330 
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
layout(location = 0) in vec3 pos;
layout(location = 1) uniform vec3 color;

uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 projection;
uniform mat4 camera;
out vec4 vcolor;
void main(void) {

  // previously average of v1,v2,v3 colors
  // vcolor      = vec4(red, green, blue, 1.0);
  gl_Position = projection * camera * translation * rotation * vec4(pos, 1.0); 
  float red = (color.x + 1) / 2;
  float green = (color.y + 1) / 2;
  float blue = (color.z + 1) / 2;

  vcolor      = vec4(red, green, blue, 1.0);
}