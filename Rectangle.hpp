class Rectangle : public AGLDrawable
{
public:
  Rectangle() : AGLDrawable(0)
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
    float z = -2.5f;
    float vertices[] = {
        0.9f, 0.9f, z,   // top right
        0.9f, -0.9f, z,  // bottom right
        -0.9f, -0.9f, z, // bottom left
        -0.9f, 0.9f, z   // top left
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


    GLint projection_loc = glGetUniformLocation(p(), "projection");
    GLint camera_loc = glGetUniformLocation(p(), "camera");

    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(camera_loc, 1, GL_FALSE, glm::value_ptr(camera));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }
};