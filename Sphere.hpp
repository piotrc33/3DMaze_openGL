class Sphere : public AGLDrawable
{
public:
  Sphere(float cx, float cy, float cz, float r) : AGLDrawable(0), cx(cx), cy(cy), cz(cz), r(r)
  {
    setShaders();
    setBuffers();
  }

  void setShaders()
  {
    compileShadersFromFile("shaders/sphere.vert", "shaders/sphere.frag");
  }

  void setBuffers()
  {
    // By default the sphere center will be in (0, 0, 0)
    bindBuffers();
    const int stackCount = 15;
    const int sectorCount = 30;
    const float PI = M_PI;

    float x, y, z, xy;                      // vertex position
    float nx, ny, nz, lengthInv = 1.0f / r; // vertex normal
    float s, t;                             // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
      stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2

      xy = r * cosf(stackAngle); // r * cos(u)
      z = r * sinf(stackAngle);  // r * sin(u)

      // add (sectorCount+1) vertices per stack
      // the first and last vertices have same position and normal, but different tex coords
      for (int j = 0; j <= sectorCount; ++j)
      {
        sectorAngle = j * sectorStep; // starting from 0 to 2pi
        // std::cout << "sector " << sectorAngle << "\n";

        // vertex position (x, y, z) with added center of sphere coordinates
        x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
        y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
      }
    }

    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
      k1 = i * (sectorCount + 1); // beginning of current stack
      k2 = k1 + sectorCount + 1;  // beginning of next stack

      for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
      {
        // 2 triangles per sector excluding first and last stacks
        // k1 => k2 => k1+1
        if (i != 0)
        {
          indices.push_back(k1);
          indices.push_back(k2);
          indices.push_back(k1 + 1);
        }

        // k1+1 => k2 => k2+1
        if (i != (stackCount - 1))
        {
          indices.push_back(k1 + 1);
          indices.push_back(k2);
          indices.push_back(k2 + 1);
        }
      }
    }

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  }

  void updatePos(glm::vec3 newPos)
  {
    cx = newPos.x;
    cy = newPos.y;
    cz = newPos.z;
  }

  void draw(glm::mat4 camera, glm::mat4 projection)
  {
    bindProgram();
    bindBuffers();

    glm::mat4 translation = glm::mat4(1.0f);
    translation = glm::translate(translation, glm::vec3(cx, cy, cz));

    GLint projection_loc = glGetUniformLocation(p(), "projection");
    GLint camera_loc = glGetUniformLocation(p(), "camera");
    GLint translation_loc = glGetUniformLocation(p(), "translation");

    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(camera_loc, 1, GL_FALSE, glm::value_ptr(camera));
    glUniformMatrix4fv(translation_loc, 1, GL_FALSE, glm::value_ptr(translation));

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  }

  float getCx() {
    return cx;
  }

  float getDistanceToPlane(glm::vec3 planeNormal, glm::vec3 p0)
  { // p0 is plane anchor point
    glm::vec3 B = {
        cx - p0.x,
        cy - p0.y,
        cz - p0.z,
    }; // vector between sphere center and anchor point

    return glm::dot(planeNormal, B) / glm::length(planeNormal);
  }

  glm::vec3 getClosestPointOnPlane(glm::vec3 planeNormal, float d)
  {
    glm::vec3 center = {cx,
                        cy,
                        cz};
    d = -d;
    planeNormal = planeNormal / glm::length(planeNormal);
    // having distance, normal and center we can calculate point coordinates
    return planeNormal * d + center;
  }

  float getR() {
    return r;
  }

private:
  // center coordinates
  float cx;
  float cy;
  float cz;

  float r; // radius
  std::vector<float> vertices;
  std::vector<int> indices;
  float offsetZ = 1.5f;
};
