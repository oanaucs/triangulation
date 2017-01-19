#include "Line.hpp"
#include <array>
#include <iostream>

Line::Line()
  : m_vao(0)
{
	std::array<float, 4> lineVertices = {1.0f, 1.0f, 2.0f, 1.0f};

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 2 * lineVertices.size()
	  , lineVertices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
  glBindVertexArray(0);
}

Line::Line(glm::vec2 min, glm::vec2 max)
  : m_vao(0)
{
	m_min = min;
	m_max = max;
	std::array<float, 4> lineVertices = {min.x, min.y, max.x, max.y};
	//std::cout<<"line vertices "<<min.x << " " << min.y << " " << max.x << " "<< max.y<<"\n";

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 2 * lineVertices.size()
	  , lineVertices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
  glBindVertexArray(0);
}

void Line::draw() const
{
  glBindVertexArray(m_vao);
  glLineWidth(3);
  glDrawArrays(GL_LINE_STRIP, 0, 2);
  glBindVertexArray(0);
}


void Line::freeVAO()
{
    glDeleteVertexArrays(1, &m_vao);
}

bool Line::equals(Line other)
{
	bool equals = false;
	if (other.m_max == m_max && other.m_min == m_min) equals = true;
	if (other.m_min == m_max && other.m_max == m_min) equals = true;

	return equals;
}
