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
    compileShadersFromFile("triangle.vert", "triangle.frag");
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
    // wzory z trójkąta równobocznego
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
    // rotating only in z axis
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
