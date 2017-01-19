#include "Rectangle.hpp"
#include<array>
#include <iostream>

namespace {

std::array<Rectangle::Vertex, 6> get_rectangleVertices(glm::vec2 min, glm::vec2 max) {

  std::array<Rectangle::Vertex, 6> vertex_array;

  // A(min.x,  min.y)
  vertex_array[0] = Rectangle::Vertex( glm::vec2( min.x, min.y));
  //vertex_array[0] = Rectangle::Vertex( glm::vec2( -0.5, -0.5));
  // B(max.x, min.y)
  vertex_array[1] = Rectangle::Vertex( glm::vec2( max.x, min.y));
  //vertex_array[1] = Rectangle::Vertex( glm::vec2( 0.5, -0.5));
  // C(max.x, max.y)
  vertex_array[2] = Rectangle::Vertex( glm::vec2( max.x, max.y));
  //vertex_array[2] = Rectangle::Vertex( glm::vec2( 0.5, 0.5));
  // C(max.x, max.y)
  vertex_array[3] = Rectangle::Vertex( glm::vec2( max.x, max.y));
  // D(min.x, max.y)
  vertex_array[4] = Rectangle::Vertex( glm::vec2( min.x, max.y));
  // A(min.x,  min.y)
  vertex_array[5] = Rectangle::Vertex( glm::vec2( min.x, min.y));

  return vertex_array;
    }
}

Rectangle::Rectangle()
  : m_vao(0)
{

	std::array<Rectangle::Vertex, 6> rectangleVertices =
    get_rectangleVertices(glm::vec2(-0.6f), glm::vec2(0.6f));
  
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * rectangleVertices.size()
              , rectangleVertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (GLvoid*)(0*sizeof(GLfloat)));
	glBindVertexArray(0);
}

Rectangle::Rectangle(glm::vec2 min, glm::vec2 max)
  : m_vao(0)
{
  std::array<Rectangle::Vertex, 6> rectangleVertices = get_rectangleVertices(min, max);

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * rectangleVertices.size()
              , rectangleVertices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (GLvoid*)(0*sizeof(GLfloat)));
  glBindVertexArray(0);
}

void Rectangle::reloadModelData(glm::vec2 lowerLeftCorner, glm::vec2 upperRightCorner)
{
	std::cout<<"reloading data "<<upperRightCorner.y<<"\n";
	//recalculate vertices
	std::array<Rectangle::Vertex, 6> rectangleVertices =
		get_rectangleVertices(lowerLeftCorner, upperRightCorner);

	glBindVertexArray(m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * rectangleVertices.size()
              , rectangleVertices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (GLvoid*)(0*sizeof(GLfloat)));
  glBindVertexArray(0);

}

void Rectangle::draw() const
{
  glBindVertexArray(m_vao);
  glDrawArrays(GL_LINE_LOOP, 0, 6);
  glBindVertexArray(0);
}


void Rectangle::freeVAO()
{
    glDeleteVertexArrays(1, &m_vao);
}

