#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Win.hpp>

struct Point
{
  float x;
  float y;
  float z;
};



int main(int argc, char *argv[])
{
  Win win;
  win.Init(800, 600, "LABIRYNT3D", 0, 33);
  if(argc > 2) {
    const int N = atoi(argv[2]);
    win.MainLoop(N);
  }
  else  
    win.MainLoop();
  return 0;
}
