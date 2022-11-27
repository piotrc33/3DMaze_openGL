#include <AGL3Window.hpp>
#include <Triangle.hpp>
#include <EquilateralTriangle.hpp>
#include <Sphere.hpp>
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

  double limit = 1 - 1.0/N;
  float offset = 2.0/N;
  float triSide = (offset*3)/(2.0*sqrt(3));

  Triangle trian(0.0f, 0.5f, -0.1f, triSide);
  Sphere ball(0.5f, 0.5f, -0.5f, 0.2);
  // trian.setStartingPos();
  // trian.translate();
  // trian.addPerspective(wd, ht);
  // trian.rotate(45.0);

  // Triangle *obstacleTriangles[N*N*N - 1];
  // int i = 0;
  // for(int x = 0; x < N; x++) {
  //   for(int y = 0; y < N; y++) {
  //     for(int z = 0; z < N; z++) {
  //       if(x == 0 && y == 0 && z == 0) continue; // leaving first space free
  //       Triangle *l = new Triangle(-limit + x*(offset), -limit + y*(offset), limit - z*(offset), triSide);
  //       // std::cout << limit - z*(offset) << "\n";
  //       obstacleTriangles[i] = l;
  //       i++;
  //     }
  //   }
  // }

  do
  {
    glClear(GL_COLOR_BUFFER_BIT);

    AGLErrors("main-loopbegin");
    // =====================================================        Drawing
    // for(auto &tri : obstacleTriangles) {
    //   tri->draw();
    // }
    // trian.rotate(1.0f);
    // trian.addPerspective(wd, ht);
    trian.draw();
    ball.draw();
    AGLErrors("main-afterdraw");

    glfwSwapBuffers(win()); // =============================   Swap buffers
    glfwPollEvents();
    // glfwWaitEvents();

    if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (glfwGetKey(win(), GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
      
    }
    else if (glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS)
    {
      
    }
  } while (glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(win()) == 0);
}