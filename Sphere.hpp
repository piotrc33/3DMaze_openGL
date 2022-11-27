class Sphere : public AGLDrawable
{
public:
  Sphere(float cx, float cy, float cz, float side) : AGLDrawable(0), cx(cx), cy(cy), cz(cz), side(side)
  {

    h = (side * sqrt(3)) / 2.0;
    setStartingPos();
    // addPerspective(wd, ht);

    setShaders();
    // std::cout << h << "\n";
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
    // dx = sqrt(((2.0 * h) / 3) * ((2.0 * h) / 3) - (h / 3) * (h / 3)); // bok trójkąta
    float x1 = 2.0*h/3 * cos(M_PI/6);
    v1 = {-x1,  -h / 3, 0.0f, 1.0f};
    v2 = { x1,  -h / 3, 0.0f, 1.0f};
    v3 = { 0.0f, 2 * h / 3, 0.0f, 1.0f};
  }

  void translate() {
    translation = glm::mat4(1.0f);
    translation = glm::translate(translation, glm::vec3(cx, cy, cz));
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)800/(float)600, 0.1f, 100.0f);
    v1 = proj * translation * v1;
    v2 = proj * translation * v2;
    v3 = proj * translation * v3;
  }

  void rotate(float angle) {
    glm::mat4 rot = glm::mat4(1.0f);
    // rotating only in z axis
    rot = glm::rotate(rot, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
    v1 = rot * v1;
    v2 = rot * v2;
    v3 = rot * v3;
    // addPerspective(800, 600);
  }

  void addPerspective(int width, int height) {
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    // std::cout << proj << "\n";
    v1 = proj * v1;
    v2 = proj * v2;
    v3 = proj * v3;
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
  // vertices
  glm::vec4 v1;
  glm::vec4 v2;
  glm::vec4 v3;
  glm::mat4 translation;
  float h;
  float dx;
  float side;
};
