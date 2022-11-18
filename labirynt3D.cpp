#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <glm/glm.hpp>

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
    // equilateral triangle
    float dx = sqrt(((2.0 * h) / 3) * ((2.0 * h) / 3) - (h / 3) * (h / 3));
    v1 = {cx - dx, cy - h / 3, cz};
    v2 = {cx + dx, cy - h / 3, cz};
    v3 = {cx, cy + 2 * h / 3, cz};
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
            const vec4 colors[]    = vec4[3](vec4(1.0, 0.0, 0.0, 1.0),
                                             vec4(0.0, 1.0, 0.0, 1.0),
                                             vec4(0.0, 0.0, 1.0, 1.0));

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

private:
  // center coordinates
  float cx;
  float cy;
  float cz;
  // vertices
  Point v1;
  Point v2;
  Point v3;
  float h = 0.3f;
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
  void MainLoop();
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
void MyWin::MainLoop()
{
  ViewportOne(0, 0, wd, ht);
  Triangle trian(0.5f, 0.0f, 0.5f);

  do
  {
    glClear(GL_COLOR_BUFFER_BIT);

    AGLErrors("main-loopbegin");
    // =====================================================        Drawing
    trian.draw();
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
  win.MainLoop();
  return 0;
}
