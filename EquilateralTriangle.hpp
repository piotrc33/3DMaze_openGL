// Trojkat równoboczny
class EquilateralTriangle : public Triangle
{
public:
  EquilateralTriangle(float cx, float cy, float cz, float side, int id) : Triangle(cx, cy, cz, side, id)
  {
    h = (side * sqrt(3)) / 2.0;
    angle = rand() % 360;
    // std::cout << angle << "\n";
    angleRatios = glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
    // setStartingPos();
    setShaders();
    setBuffers();
  }

  void setStartingPos() {
    // wzory z trójkąta równobocznego
    // dx = sqrt(((2.0 * h) / 3) * ((2.0 * h) / 3) - (h / 3) * (h / 3)); // bok trójkąta
    float x1 = 2.0*h/3 * cos(M_PI/6);

    positions = {
        -x1, -h / 3, 0.0f,
        x1,  -h / 3, 0.0f,
        0.0f, 2 * h / 3, 0.0f
      };
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
};
