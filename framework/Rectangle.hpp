#ifndef RECTANGLE_HPP
#define TRIANGLE_HPP

#include <GL/glew.h>
#ifdef __APPLE__
# define __gl3_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Rectangle
{
public:
  struct Vertex
  {
    Vertex()
      : position(0.0, 0.0)
    {}

    Vertex(glm::vec2 pos)
      : position(pos)
    {}

    glm::vec2 position;
  };

  Rectangle();
  Rectangle(glm::vec2 min, glm::vec2 max);
  void draw() const;
  void reloadModelData(glm::vec2 lowerLeftCorner, glm::vec2 upperRightCorner);
  void freeVAO();

private:
  GLuint m_vao;
  GLuint m_vbo;
};

#endif
