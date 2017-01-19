#ifndef LINE_HPP
#define LINE_HPP

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

class Line
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

  Line();
  Line(glm::vec2 min, glm::vec2 max);
  void draw() const;
  void freeVAO();
  bool equals(Line other);

  glm::vec2 m_min;
  glm::vec2 m_max;

private:
  GLuint m_vao;
};

inline bool operator == (const Line & e1, const Line & e2)
{
	return (e1.m_min == e2.m_min && e1.m_max == e2.m_max) || (e1.m_min == e2.m_max && e1.m_max == e2.m_min);
}

#endif
