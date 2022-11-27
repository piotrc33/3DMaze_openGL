#include <AGL3Drawable.hpp>


// chyba żeby zrobić jak należy to trzeba wierzchołki do buffera przekazywać
// przykład tego możńa w MyCross znaleźć

class Triangle : public AGLDrawable
{
public:
  Triangle(float cx, float cy, float cz, float side) : AGLDrawable(0), cx(cx), cy(cy), cz(cz), side(side)
  {
    h = (side * sqrt(3)) / 2.0;
    angle = rand() % 360;
    // std::cout << angle << "\n";
    angleRatios = glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
    setStartingPos();
    setShaders();
    setBuffers();
  }
  void setShaders()
  {
    compileShadersFromFile("triangle.vert", "triangle.frag");
  }
  void setBuffers()
  {
    bindBuffers();

    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,        // attribute 0, must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // 24,             // stride
        (void *)0 // array buffer offset
    );
  }
  void draw()
  {
    bindProgram();
    bindBuffers(); 
    glm::mat4 translation = glm::mat4(1.0f);
    translation = glm::translate(translation, glm::vec3(cx, cy, cz - 1.5f));

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(angle), glm::vec3(0.5, 1.0, 0.1));

    glm::mat4 projection    = glm::mat4(1.0f);
    // The range is from -0.1 to -100.0 on z axis to render
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    GLint translation_loc = glGetUniformLocation(p(), "translation");
    GLint rotation_loc = glGetUniformLocation(p(), "rotation");
    GLint projection_loc = glGetUniformLocation(p(), "projection");
    glUniformMatrix4fv(translation_loc, 1, GL_FALSE, glm::value_ptr(translation));
    glUniformMatrix4fv(rotation_loc, 1, GL_FALSE, glm::value_ptr(rotation));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
    // glUniform3f(1, v1.x, v1.y, v1.z);
    // glUniform3f(2, v2.x, v2.y, v2.z);
    // glUniform3f(3, v3.x, v3.y, v3.z);

    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  void setStartingPos() {
    // wzory z trójkąta równobocznego
    // dx = sqrt(((2.0 * h) / 3) * ((2.0 * h) / 3) - (h / 3) * (h / 3)); // bok trójkąta
    float x1 = 2.0*h/3 * cos(M_PI/6);
    v1 = {-x1,  -h / 3, 0.0f, 1.0f};
    v2 = { x1,  -h / 3, 0.0f, 1.0f};
    v3 = { 0.0f, 2 * h / 3, 0.0f, 1.0f};


    positions = {
        -x1, -h / 3, 0.0f,
        x1,  -h / 3, 0.0f,
        0.0f, 2 * h / 3, 0.0f
      };
  }

  void translate() {
    // translation = glm::mat4(1.0f);
    // translation = glm::translate(translation, glm::vec3(cx, cy, cz));
    // v1 = translation * v1;
    // v2 = translation * v2;
    // v3 = translation * v3;
  }

  void rotate(float angle) {
    glm::mat4 rot = glm::mat4(1.0f);
    rot = glm::rotate(rot, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
    v1 = rot * v1;
    v2 = rot * v2;
    v3 = rot * v3;
    // addPerspective(800, 600);
  }

  void addPerspective(int width, int height) {
    // glm::mat4 proj = glm::mat4(1.0f); // here might have been a problem
    // glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    // // std::cout << proj << "\n";
    // v1 = proj * v1;
    // v2 = proj * v2;
    // v3 = proj * v3;
  }

  float getDx() {
    return dx;
  }

  float getHeight() {
    return h;
  }

private:
  // center coordinates
  float cx;
  float cy;
  float cz;
  float angle; // in degrees
  glm::vec3 angleRatios;
  // vertices
  glm::vec4 v1;
  glm::vec4 v2;
  glm::vec4 v3;
  std::vector<float> positions;
  float h;
  float dx;
  float side;
};
