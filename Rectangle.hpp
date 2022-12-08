class Rectangle : public AGLDrawable
{
public:
  Rectangle(float cx, float cy, float cz, float angle, glm::vec3 angleRatios) : AGLDrawable(0), cx(cx), cy(cy), cz(cz), angle(angle), angleRatios(angleRatios)
  {
    setShaders();
    setBuffers();
  }

  void setShaders()
  {
    compileShadersFromFile("shaders/rectangle.vert", "shaders/rectangle.frag");
  }

  void setBuffers()
  {
    bindBuffers();
    float side = 2 * 1.0f;

    float vertices[] = {
        defaultX, defaultY, defaultZ,               // top left
        defaultX + side, defaultY, defaultZ,        // top right
        defaultX + side, defaultY - side, defaultZ, // bottom right
        defaultX, defaultY - side, defaultZ, // bottom left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  }
  void draw(glm::mat4 camera, glm::mat4 projection)
  {
    bindProgram();
    bindBuffers();

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(angle), angleRatios);

    glm::mat4 translation = glm::mat4(1.0f);
    translation = glm::translate(translation, glm::vec3(cx - defaultX, cy - defaultY, cz - defaultZ));

    GLint projection_loc = glGetUniformLocation(p(), "projection");
    GLint camera_loc = glGetUniformLocation(p(), "camera");
    GLint translation_loc = glGetUniformLocation(p(), "translation");
    GLint rotation_loc = glGetUniformLocation(p(), "rotation");

    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(camera_loc, 1, GL_FALSE, glm::value_ptr(camera));
    glUniformMatrix4fv(translation_loc, 1, GL_FALSE, glm::value_ptr(translation));
    glUniformMatrix4fv(rotation_loc, 1, GL_FALSE, glm::value_ptr(rotation));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

private:
  float defaultX = 0.0f;
  float defaultY = 0.0f;
  float defaultZ = 0.0f;
  float cx;
  float cy;
  float cz;
  float angle;
  glm::vec3 angleRatios;
};