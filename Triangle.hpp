#include <AGL3Drawable.hpp>

class Triangle : public AGLDrawable
{
public:
  Triangle(float cx, float cy, float cz) : AGLDrawable(0), cx(cx), cy(cy), cz(cz)
  {

    setStartingPos();
    rotate(rand() % 10);
    translate();
    setShaders();
  }
  void setShaders()
  {
    compileShaders(R"END(

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

      )END",
                   R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         in  vec4 vcolor;
         out vec4 color;

         void main(void) {
            color = vcolor;
         } 

      )END");
  }
  void draw()
  {
    bindProgram();
    bindBuffers(); 
    glUniform3f(1, v1.x, v1.y, v1.z);
    glUniform3f(2, v2.x, v2.y, v2.z);
    glUniform3f(3, v3.x, v3.y, v3.z);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  void setStartingPos() {
    dx = sqrt(((2.0 * h) / 3) * ((2.0 * h) / 3) - (h / 3) * (h / 3));
    v1 = {-dx,  -h / 3, 0.0f, 1.0f};
    v2 = { dx,  -h / 3, 0.0f, 1.0f};
    v3 = { 0.0f, 2 * h / 3, 0.0f, 1.0f};
  }

  void translate() {
    translation = glm::mat4(1.0f);
    translation = glm::translate(translation, glm::vec3(cx, cy, cz));
    v1 = translation * v1;
    v2 = translation * v2;
    v3 = translation * v3;
  }

  void rotate(float angle) {
    glm::mat4 rot = glm::mat4(1.0f);
    rot = glm::rotate(rot, angle, glm::vec3(0.0, 0.0, 1.0));
    v1 = rot * v1;
    v2 = rot * v2;
    v3 = rot * v3;
  }

  float getDx() {
    return dx;
  }

private:
  // center coordinates
  float cx;
  float cy;
  float cz;
  // vertices
  glm::vec4 v1;
  glm::vec4 v2;
  glm::vec4 v3;
  glm::mat4 translation;
  float h = 0.3f;
  float dx;
};
