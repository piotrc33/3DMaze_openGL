#include <AGL3Window.hpp>
#include <Triangle.hpp>
#include <Rectangle.hpp>
#include <EquilateralTriangle.hpp>
#include <glm/glm.hpp>

float lastX = 400;
float lastY = 300;
float yaw = -90.0f; // horizontal rotation
float pitch = 0.0f; // vertical rotation

float cameraSpeed = 0.01;
float angleSpeed = 1.0f;

static void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
  lastX = xpos;
  lastY = ypos;

  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;
}

class Win : public AGLWindow
{
public:
  // float lastX = 400, lastY = 300;
  Win(){};
  Win(int _wd, int _ht, const char *name, int vers, int fullscr = 0)
      : AGLWindow(_wd, _ht, name, vers, fullscr){};
  virtual void KeyCB(int key, int scancode, int action, int mods);
  void MainLoop(int N);
  void handleBallRotation();

private:
};

void Win::KeyCB(int key, int scancode, int action, int mods)
{
  AGLWindow::KeyCB(key, scancode, action, mods); // f-key full screen switch
  if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS)
  {
    ; // do something
  }
  if (key == GLFW_KEY_HOME && (action == GLFW_PRESS))
  {
    ; // do something
  }
}

void Win::handleBallRotation() {
  // angle steering
  if (glfwGetKey(win(), GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    yaw += angleSpeed;
  }
  else if (glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    yaw -= angleSpeed;
  }

  if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS)
  {
    if (pitch < 89)
      pitch += angleSpeed;
  }
  else if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    if (pitch > -89)
      pitch -= angleSpeed;
  }
}

void Win::MainLoop(int N = 10)
{
  ViewportOne(0, 0, wd, ht);
  glEnable(GL_DEPTH_TEST);
  glfwSetInputMode(win(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // float lastX = 400, lastY = 300;
  glfwSetCursorPosCallback(win(), mouseCallback);
  
  double limit = 1 - 1.0 / N;
  float offset = 2.0 / N;
  float triSide = (offset * 3) / (2.0 * sqrt(3));

  // CHANGING VIEWS
  bool spaceClicked = false;
  enum viewMode
  {
    FIRST_PERSON = 0,
    THIRD_PERSON = 1,
    TOP_VIEW = 2
  };

  // viewMode vm = FIRST_PERSON;
  viewMode vm = FIRST_PERSON;

  glm::mat4 camera = glm::mat4(1.0f);

  glm::vec3 cameraPos = {0.0f, 0.0f, 0.0f};
  // glm::vec3 cameraPos = {-2.0297f, -0.669669f, -0.637569f};
  glm::vec3 cameraUp = {0.0f, 1.0f, 0.0f};
  glm::vec3 cameraFront = {0.0f, 0.0f, -1.0f}; // target
  // camera rotation angles, yaw = around y axis, pitch ar. x axis


  // glm::mat4 topCamera = glm::mat4(1.0f);
  glm::vec3 topCameraPos = {0.0f, 2.7f, -1.8f};
  glm::vec3 topCameraFront = {0.0f, -1.0f, 0.1f};
  float topYaw = -270.0f;  // horizontal rotation
  float topPitch = -84.0f; // vertical rotation

  // ball starting position
  glm::vec3 ballPos = {
      -0.9f,
      -0.9f,
      -0.6f,
  };
  // glm::vec3 ballPos = cameraPos;

  Sphere ball(ballPos.x, ballPos.y, ballPos.z, triSide / 4);
  Rectangle rect1(-1.0f, 1.0f, -2.5f, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  Rectangle rect2(-1.0f, 1.0f, -0.5f, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  Rectangle rect3(-1.0f, 1.0f, -0.5f, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  Rectangle rect4(1.0f, 1.0f, -0.5f, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  Rectangle rect5(-1.0f, -1.0f, -0.5f, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  // Rectangle rect2;

  // now obstacle Z coordinates are from -0.6 to -2.6
  Triangle *obstacleTriangles[N * N * N - 1];
  int i = 0;
  for (int x = 0; x < N; x++)
  {
    for (int y = 0; y < N; y++)
    {
      for (int z = 0; z < N; z++)
      {
        if (x == 0 && y == 0 && z == 0)
          continue; // leaving first space free

        Triangle *l = new Triangle(-limit + x * (offset), -limit + y * (offset), limit - z * (offset), triSide, i);
        obstacleTriangles[i] = l;

        i++;
      }
    }
  }

  do
  {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glfwSetWindowSizeCallback(win(), [](GLFWwindow *window, int width, int height) {
    //   // int smaller = width < height ? width : height;
    //   if(width < height)
    //     glViewport(0, 0, width, 3*width/4);
    //   else
    //     glViewport(0, 0, 4*height/3, height);
    // });

    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    ball.updatePos(ballPos); // later on change to cameraPos, keeping the radius in mind
    if (vm == FIRST_PERSON)
      cameraPos = ballPos;
    if (vm == THIRD_PERSON)
      cameraPos = ballPos - 0.5f * cameraFront;
    if (vm == TOP_VIEW)
    {
      camera = glm::lookAt(topCameraPos,                  // position
                           topCameraPos + topCameraFront, // target
                           cameraUp);                     // up
    }
    else
    {
      camera = glm::lookAt(cameraPos,               // position
                           cameraPos + cameraFront, // target
                           cameraUp);               // up
    }

    // std::cout << "cameraPos: " << cameraPos.x << " " << cameraPos.y << " "<< cameraPos.z << "\n";
    // std::cout << "cameraFront: " << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << "\n";
    // std::cout << "yaw: " << yaw << " pitch: " << pitch << "\n";

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    AGLErrors("main-loopbegin");
    // =====================================================        Drawing
    for (auto &tri : obstacleTriangles)
    {
      tri->draw(camera, projection);

      bool col = tri->checkCollision(&ball);
      if (col)
      {
        if (tri->getId() == 998)
        {
          std::cout << "You won! Your time: " << glfwGetTime() << " seconds"
                                                                  "\n";
        }
        else
          std::cout << "Collision! "
                    << "\n";
      }
    }


    // ************* DRAWING*******************
    rect1.draw(camera, projection);
    rect2.draw(camera, projection);
    rect3.draw(camera, projection);
    rect4.draw(camera, projection);
    rect5.draw(camera, projection);
    ball.draw(camera, projection);
    AGLErrors("main-afterdraw");

    glfwSwapBuffers(win()); // =============================   Swap buffers
    glfwPollEvents();
    // glfwWaitEvents();

    // ROTATION OF BALL
    handleBallRotation();

    // handling view changes
    if (glfwGetKey(win(), GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
      spaceClicked = false;
    }


    if (glfwGetKey(win(), GLFW_KEY_Q) == GLFW_PRESS)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (glfwGetKey(win(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
      if (vm == 0 && spaceClicked == false)
      {
        // setting camera to be behind ball
        cameraPos = ballPos - cameraFront;

        vm = THIRD_PERSON;
        spaceClicked = true;
      }
      if (vm == 1 && spaceClicked == false)
      {
        vm = TOP_VIEW;
        spaceClicked = true;
      }
      if (vm == 2 && spaceClicked == false)
      {
        vm = FIRST_PERSON;
        spaceClicked = true;
      }
    }
    else if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS)
    {
      // cameraPos += cameraSpeed * cameraFront;
      ballPos += cameraSpeed * cameraFront;
      // std::cout << "A clicked \n";
    }
    else if (glfwGetKey(win(), GLFW_KEY_Z) == GLFW_PRESS)
    {
      ballPos -= cameraSpeed * cameraFront;
      // cameraPos -= cameraSpeed * cameraFront;
      // std::cout << "Z clicked \n";
    }
    else if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS)
    {
      cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    else if (glfwGetKey(win(), GLFW_KEY_X) == GLFW_PRESS)
    {
      cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    
  } while (glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(win()) == 0);
}
