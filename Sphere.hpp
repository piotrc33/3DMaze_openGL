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
    compileShadersFromFile("sphere.vert", "sphere.frag");
  }

  void draw()
  {
    bindProgram();
    bindBuffers();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  }

void setBuffers() {
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
      xy = r * cosf(stackAngle);           // r * cos(u)
      z = r * sinf(stackAngle);            // r * sin(u)

      // add (sectorCount+1) vertices per stack
      // the first and last vertices have same position and normal, but different tex coords
      for (int j = 0; j <= sectorCount; ++j)
      {
        sectorAngle = j * sectorStep; // starting from 0 to 2pi

        // vertex position (x, y, z)
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

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

private:
  // center coordinates
  float cx;
  float cy;
  float cz;

  float r; // radius
  std::vector<float> vertices;
  std::vector<int> indices;
};
