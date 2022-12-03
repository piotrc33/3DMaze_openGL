#include <AGL3Window.hpp>
#include <Triangle.hpp>
#include <Rectangle.hpp>
#include <EquilateralTriangle.hpp>
// #include <Sphere.hpp>
#include <glm/glm.hpp>

class Win : public AGLWindow
{
public:
  Win(){};
  Win(int _wd, int _ht, const char *name, int vers, int fullscr = 0)
      : AGLWindow(_wd, _ht, name, vers, fullscr){};
  virtual void KeyCB(int key, int scancode, int action, int mods);
  // void MainLoop();
  void MainLoop(int N);
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

void Win::MainLoop(int N = 10)
{
  ViewportOne(0, 0, wd, ht);
  glEnable(GL_DEPTH_TEST);

  double limit = 1 - 1.0 / N;
  float offset = 2.0 / N;
  float triSide = (offset * 3) / (2.0 * sqrt(3));

  // CHANGING VIEWS
  bool spaceClicked = false;
  enum viewMode
  {
    FIRST_PERSON = 0,
    THIRD_PERSON = 1,
  };

  viewMode vm = FIRST_PERSON;

  glm::mat4 camera = glm::mat4(1.0f);
  glm::vec3 cameraPos = {0.0f, 0.0f, 0.0f};
  // glm::vec3 cameraPos = {-2.0297f, -0.669669f, -0.637569f};
  glm::vec3 cameraUp = {0.0f, 1.0f, 0.0f};
  glm::vec3 cameraFront = {0.0f, 0.0f, -1.0f}; // target
  // glm::vec3 cameraFront = {0.998021f, -0.052336f, -0.0348517f}; // target
  float cameraSpeed = 0.01;

  // camera angles, yaw = around y axis, pitch ar. x axis
  float yaw = -90.0f, pitch = 0.0f;
  // float yaw = -3.0f, pitch = -9.0f;
  float angleSpeed = 1.0f;

  // glm::vec3 ballPos = {
  //     -0.9f,
  //     -0.9f,
  //     -0.6f,
  // };
  glm::vec3 ballPos = cameraPos;

  // Triangle trian(-0.6f, -0.3f, 0.0f, triSide);
  Sphere ball(ballPos.x, ballPos.y, ballPos.z, triSide / 4);
  Rectangle rect1;

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

        if (i == 459)
        {
          Triangle *l = new Triangle(-limit + x * (offset), -limit + y * (offset), limit - z * (offset), triSide*5, i);
          obstacleTriangles[i] = l;
        }
        else
        {
          Triangle *l = new Triangle(-limit + x * (offset), -limit + y * (offset), limit - z * (offset), triSide, i);
          obstacleTriangles[i] = l;
        }

        i++;
      }
    }
  }

  do
  {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // std::cout << "length: " << length(glm::vec2(1.0f, 1.0f));

    cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    camera = glm::lookAt(cameraPos,               // position
                         cameraPos + cameraFront, // target
                         cameraUp);               // up

    ballPos = cameraPos + cameraFront;
    // ballPos.z -= 0.1f;
    if (vm == FIRST_PERSON)
      ball.updatePos(cameraPos); // later on change to cameraPos, keeping the radius in mind
      // ball.updatePos(ballPos); // later on change to cameraPos, keeping the radius in mind
    if (vm == THIRD_PERSON)
      // ball.updatePos(cameraPos); // later on change to cameraPos, keeping the radius in mind
      ball.updatePos(ballPos); // later on change to cameraPos, keeping the radius in mind

    glm::mat4 projection = glm::mat4(1.0f);
    // The range is from -0.1 to -100.0 on z axis to render
    projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // std::cout << "cameraPos: " << cameraPos.x << " " << cameraPos.y << " "<< cameraPos.z << "\n";
    // std::cout << "cameraFront: " << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << "\n";
    // std::cout << "yaw: " << yaw << " pitch: " << pitch << "\n";

    AGLErrors("main-loopbegin");
    // =====================================================        Drawing
    for (auto &tri : obstacleTriangles)
    {
      // tri->setV1V2();
      tri->draw(camera, projection);
      
      bool col = tri->checkCollision(&ball);
      if(col)
        std::cout << "Collision! " << glfwGetTime() << "\n";
    }
    // trian.draw(camera);


    // ******** CHECKING COLLISION ***********
    Triangle *t = obstacleTriangles[459];

    // glm::vec3 normal = t->getNormalVec();
    // glm::vec3 vc = t->getCenterPoint();
    // float d1 = ball.getDistanceToPlane(normal, vc);
    // glm::vec3 p = ball.getClosestPointOnPlane(normal, d1);
    // float d2 = t->getDistanceToPoint(p);

    // float D = sqrt(d1*d1 + d2*d2);

    // float R = ball.getR();
    // // std::cout << d2 << "\n";
    // if(D < R) {
    //   // std::cout << d1 << " " << d2 << " " << D << "\n";
    //   std::cout << "Collision! " << glfwGetTime() << "\n";
    // }
    // std::cout << "original: " << p.x << " " << p.y << " " << p.z << " \n";

    

    // Sphere ball2(p.x, p.y, p.z, triSide / 8);
    // ball2.draw(camera, projection);

    // bool col = t->checkCollision(&ball);
    // if (col)
    //   std::cout << "Collision! " << glfwGetTime() << "\n";

    rect1.draw(camera, projection);
    ball.draw(camera, projection);
    AGLErrors("main-afterdraw");

    glfwSwapBuffers(win()); // =============================   Swap buffers
    glfwPollEvents();
    // glfwWaitEvents();

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
    else if (glfwGetKey(win(), GLFW_KEY_ENTER) == GLFW_PRESS)
    {
      ballPos.x += cameraSpeed;
    }
    else if (glfwGetKey(win(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
      if (vm == 0 && spaceClicked == false)
      {
        vm = THIRD_PERSON;
        std::cout << "third person mode \n";
        spaceClicked = true;
      }
      if (vm == 1 && spaceClicked == false)
      {
        vm = FIRST_PERSON;
        std::cout << "first person mode \n";
        spaceClicked = true;
      }
    }
    else if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS)
    {
      cameraPos += cameraSpeed * cameraFront;
    }
    else if (glfwGetKey(win(), GLFW_KEY_Z) == GLFW_PRESS)
    {
      cameraPos -= cameraSpeed * cameraFront;
    }
    else if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS)
    {
      cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    else if (glfwGetKey(win(), GLFW_KEY_X) == GLFW_PRESS)
    {
      cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    else if (glfwGetKey(win(), GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
      yaw += angleSpeed;
    }
    else if (glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS)
    {
      yaw -= angleSpeed;
    }
    else if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS)
    {
      pitch += angleSpeed;
    }
    else if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS)
    {
      pitch -= angleSpeed;
    }
  } while (glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(win()) == 0);
}