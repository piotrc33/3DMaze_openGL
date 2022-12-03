#include <AGL3Drawable.hpp>
#include <Sphere.hpp>

// chyba żeby zrobić jak należy to trzeba wierzchołki do buffera przekazywać
// przykład tego możńa w MyCross znaleźć

class Triangle : public AGLDrawable
{
public:
  Triangle(float cx, float cy, float cz, float side, int id) : AGLDrawable(0), cx(cx), cy(cy), cz(cz), side(side), id(id)
  {
    h = (side * sqrt(3)) / 2.0;
    angle = rand() % 360;
    angleRatios = glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);

    setStartingPos();
    setV1V2();
    setShaders();
    setBuffers();
  }

  void setShaders()
  {
    compileShadersFromFile("shaders/triangle.vert", "shaders/triangle.frag");
  }

  void setBuffers()
  {
    bindBuffers();

    glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,        // attribute 0, must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // 24,             // stride
        (void *)0 // array buffer offset
    );
  }
  void draw(glm::mat4 camera, glm::mat4 projection)
  {
    bindProgram();
    bindBuffers();

    glUniform3f(1, cx, cy, cz);
    glm::mat4 translation = glm::mat4(1.0f);
    translation = glm::translate(translation, glm::vec3(cx, cy, cz - offsetZ));

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(angle), angleRatios);

    GLint translation_loc = glGetUniformLocation(p(), "translation");
    GLint rotation_loc = glGetUniformLocation(p(), "rotation");
    GLint projection_loc = glGetUniformLocation(p(), "projection");
    GLint camera_loc = glGetUniformLocation(p(), "camera");

    glUniformMatrix4fv(translation_loc, 1, GL_FALSE, glm::value_ptr(translation));
    glUniformMatrix4fv(rotation_loc, 1, GL_FALSE, glm::value_ptr(rotation));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(camera_loc, 1, GL_FALSE, glm::value_ptr(camera));

    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  void setStartingPos()
  {
    // wzory z trójkąta równobocznego
    // dx = sqrt(((2.0 * h) / 3) * ((2.0 * h) / 3) - (h / 3) * (h / 3)); // bok trójkąta
    float x1 = 2.0 * h / 3 * cos(M_PI / 6); // x1 to połowa boku trójkąta

    positions = {
        -x1, -h / 3, 0.0f,
        x1, -h / 3, 0.0f,
        0.0f, 2 * h / 3, 0.0f};
  }

  float getHeight()
  {
    return h;
  }

  int getId() {
    return id;
  }

  void setV1V2()
  {
    // Only rotation affects the normal vector!!

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(angle), angleRatios);

    glm::vec4 v14 = {positions[0],
                     positions[1],
                     positions[2],
                     1.0};

    glm::vec4 v24 = {positions[3],
                     positions[4],
                     positions[5],
                     1.0};

    v14 = rotation * v14;
    v24 = rotation * v24;

    v1 = {v14.x, v14.y, v14.z};
    v2 = {v24.x, v24.y, v24.z};

    // v1 = {positions[0],
    //       positions[1],
    //       positions[2]};

    // v2 = {positions[3],
    //       positions[4],
    //       positions[5]};
  }

  glm::vec3 getNormalVec()
  {
    // std::cout << v1.x << " " << v1.y << " " << v1.z << " \n";
    glm::vec3 n = glm::cross(v1, v2);
    return n;
  }

  glm::vec3 getV1()
  {
    return {
        positions[0],
        positions[1],
        positions[2],
    };
  }

  glm::vec3 getCenterPoint()
  {
    return {
        cx,
        cy,
        cz - offsetZ,
    };
  }

  float getDistanceFromSideToPoint(float x1, float y1, float x2, float y2, glm::vec2 p)
  {
    // (x1, y1) is first point of side, analogous with second one
    // returning signed distance, depends on which side the point is
    // https://www.wikiwand.com/en/Distance_from_a_point_to_a_line#Cartesian_coordinates
    // return ((x2 - x1)*(y1 - p.y) - (x1 - p.x)*(y2 - y1)) / side;
    // https://www.geeksforgeeks.org/minimum-distance-from-a-point-to-the-line-segment-using-vectors/

    glm::vec3 AB = { x2 - x1, y2 - y1, 0.0f};
    glm::vec3 BE = {p.x - x2, p.y - y2, 0.0f};
    glm::vec3 AE = {p.x - x1, p.y - y1, 0.0f};

    float abbe = glm::dot(AB, BE);
    float abae = glm::dot(AB, AE);

    if(abbe > 0.0f) { // B is closest point
      return glm::length(BE);
    }
    if(abae < 0.0f) {
      return glm::length(AE); // A is closest point
    }

    return ((x2 - x1) * (y1 - p.y) - (x1 - p.x) * (y2 - y1)) / side;

    // return 0.0f;
    // return ((y2 - y1) * p.x - (x2 - x1) * p.y + x2 * y1 - y2 * x1) / side;
  }

  float getDistanceToPoint(glm::vec3 p) {
    // projecting the closest point on plane onto 2D starting plane, with 0.0 z coordinate
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(-angle), angleRatios);

    glm::mat4 translation = glm::mat4(1.0f);
    translation = glm::translate(translation, glm::vec3(-cx, -cy, -cz + offsetZ));

    // parsing to vec4 to be able to perform transformation with mat4
    glm::vec4 p4 = {p, 1.0f};
    p4 = rotation * translation * p4; // when going back inverse order is required?

    // std::cout << "rotated: " << p4.x << " " << p4.y << " " << p4.z << " \n";

    // vertices on 2D starting plane
    glm::vec2 A = { positions[0],
                     positions[1]};
    glm::vec2 B = { positions[3],
                     positions[4]};
    glm::vec2 C = { positions[6],
                     positions[7] };

    // distances from every side
    float AB = getDistanceFromSideToPoint(A.x, A.y, B.x, B.y, p4);
    float AC = getDistanceFromSideToPoint(A.x, A.y, C.x, C.y, p4);
    float BC = getDistanceFromSideToPoint(B.x, B.y, C.x, C.y, p4);

    // std::cout << AB << " " << AC << " " << BC << " \n";

    // empiric checking if point is inside triangle
    // for this combination point is in triangle
    if(AB < 0.0f && AC > 0.0f && BC < 0.0f) {
      return 0.0f;
    } else {
      return std::min(std::abs(AB), std::min(std::abs(AC), std::abs(BC)));
    }
  }

  bool checkCollision(Sphere *ball) {
    glm::vec3 normal = getNormalVec();
    glm::vec3 vc = getCenterPoint();
    float d1 = ball->getDistanceToPlane(normal, vc);
    glm::vec3 p = ball->getClosestPointOnPlane(normal, d1);
    float d2 = getDistanceToPoint(p);

    float D = sqrt(d1 * d1 + d2 * d2);
    float R = ball->getR();

    if (D < R)
      return true;
    return false;
  }

private:
  int id;
  // center coordinates
  float cx;
  float cy;
  float cz;
  float angle;           // in degrees
  glm::vec3 angleRatios; // how it should be turned in each axis
  glm::vec3 v1;
  glm::vec3 v2;
  // vertices
  std::vector<float>
      positions;
  float h;
  float side;
  float offsetZ = 1.5f;
};
