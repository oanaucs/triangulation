#include "circle.hpp"
#include <array>
#include <iostream>

Circle::Circle() : m_vao(0)
{

}

Circle::Circle(glm::vec2 const center, float const radius)
	: m_vao(0), m_samplePointsNo(20)
{
	createCircleVertices(center, radius);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_circleVertices.size()
	            , m_circleVertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), nullptr);
	glBindVertexArray(0);
}

Circle::Circle(std::vector<float> const &circleVertices)
  : m_vao(0), m_samplePointsNo(20)
{
	m_circleVertices = circleVertices;
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_circleVertices.size()
	            , m_circleVertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), nullptr);
	glBindVertexArray(0);
}

Circle::~Circle()
{

}

void Circle::createCircleVertices(glm::vec2 const center, float const radius)
{
	for (unsigned int index = 0; index < 20; index++)   
	{
		float theta = 2.0f * PI * float(index) / 20.0f; //get the current angle 
        float x = radius * cosf(theta); //calculate the x component 
        float y = radius * sinf(theta); //calculate the y component 
		m_circleVertices.push_back(x + center.x);
		m_circleVertices.push_back(y + center.y);
    }
}

void Circle::draw() const
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_LINE_LOOP, 0, m_circleVertices.size() / 2);
	glBindVertexArray(0);
}


void Circle::freeVAO()
{
    glDeleteVertexArrays(1, &m_vao);
}

