#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <AGL3Window.hpp>
#include <AGL3Drawable.hpp>

struct Point
{
  float x;
  float y;
  float z;
};

class Triangle : public AGLDrawable
{
public:
  Triangle(float cx, float cy, float cz) : AGLDrawable(0), cx(cx), cy(cy), cz(cz)
  {

    setStartingPos();
    rotate(rand() % 10);
    translate();
    setShaders();
  }
  void setShaders()
  {
    compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 1) uniform vec3 v1;
         layout(location = 2) uniform vec3 v2;
         layout(location = 3) uniform vec3 v3;
         out vec4 vcolor;
         void main(void) {
            const vec3 vertices[3] = vec3[3](vec3( v1.x, v1.y, v1.z),
                                             vec3( v2.x, v2.y, v2.z),
                                             vec3( v3.x, v3.y, v3.z));

            float red = (v1.x + v2.x + v3.x)/3;
            red = (red + 1) / 2;
            float green = (v1.y + v2.y + v3.y)/3;
            green = (green + 1) / 2;
            float blue = (v1.z + v2.z + v3.z)/3;
            blue = (blue + 1) / 2;
            const vec4 colors[]    = vec4[3](vec4(red, green, blue, 1.0),
                                             vec4(red, green, blue, 1.0),
                                             vec4(red, green, blue, 1.0));

            vcolor      = colors[gl_VertexID];
            gl_Position = vec4(vertices[gl_VertexID], 1.0); 
         }

      )END",
                   R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         in  vec4 vcolor;
         out vec4 color;

         void main(void) {
            color = vcolor;
         } 

      )END");
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
    dx = sqrt(((2.0 * h) / 3) * ((2.0 * h) / 3) - (h / 3) * (h / 3));
    v1 = {-dx,  -h / 3, 0.0f, 1.0f};
    v2 = { dx,  -h / 3, 0.0f, 1.0f};
    v3 = { 0.0f, 2 * h / 3, 0.0f, 1.0f};
  }

  void translate() {
    translation = glm::mat4(1.0f);
    translation = glm::translate(translation, glm::vec3(cx, cy, cz));
    v1 = translation * v1;
    v2 = translation * v2;
    v3 = translation * v3;
  }

  void rotate(float angle) {
    glm::mat4 rot = glm::mat4(1.0f);
    rot = glm::rotate(rot, angle, glm::vec3(0.0, 0.0, 1.0));
    v1 = rot * v1;
    v2 = rot * v2;
    v3 = rot * v3;
  }

  float getDx() {
    return dx;
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
  float h = 0.3f;
  float dx;
};

// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow
{
public:
  MyWin(){};
  MyWin(int _wd, int _ht, const char *name, int vers, int fullscr = 0)
      : AGLWindow(_wd, _ht, name, vers, fullscr){};
  virtual void KeyCB(int key, int scancode, int action, int mods);
  // void MainLoop();
  void MainLoop(int N);
};

// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods)
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

// ==========================================================================
void MyWin::MainLoop(int N = 10)
{
  ViewportOne(0, 0, wd, ht);
  Triangle trian(0.5f, -0.3f, 0.5f);
  trian.rotate(glm::radians(45.0f));

  double limit = 1 - 1.0/N;
  double triWidth = trian.getDx();

  Triangle *obstacleTriangles[N*N*N];
  int i = 0;
  for(int x = 0; x < N; x++) {
    for(int y = 0; y < N; y++) {
      for(int z = 0; z < N; z++) {
        Triangle *l = new Triangle(-limit + x*(triWidth + 0.00001)*2, -limit + y*(triWidth + 0.00001)*2, -limit + z*(triWidth + 0.00001)*2);
        obstacleTriangles[i] = l;
        i++;
      }
    }
  }

  do
  {
    glClear(GL_COLOR_BUFFER_BIT);

    // trian.translate();
    // trian.rotate(glm::radians(1.0f));

    AGLErrors("main-loopbegin");
    // =====================================================        Drawing
    for(auto &tri : obstacleTriangles) {
      tri->draw();
      }
    // trian.draw();
    // cross.draw(tx,ty);
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
      // tx += 0.01;
    }
    else if (glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS)
    {
      // tx -= 0.01;
    }
  } while (glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(win()) == 0);
}

int main(int argc, char *argv[])
{
  MyWin win;
  win.Init(800, 600, "LABIRYNT3D", 0, 33);
  if(argc > 2) {
    const int N = atoi(argv[2]);
    win.MainLoop(N);
  }
  else  
    win.MainLoop();
  return 0;
}
