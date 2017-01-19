#ifndef CIRCLE_HPP
#define CIRCLE_HPP

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
#include <vector>

#define PI 3.1415926f

class Circle
{
public:
	Circle();
	Circle(glm::vec2 const center, float const radius);
	Circle(std::vector<float> const &circleVertices);
	~Circle();
	//void createVertices(void);
	//void setRadius(float &radius);
	//void setSamplePointsNumber(unsigned int &samplePointsNo);
	//void setCenter(glm::vec2 &center);
	void draw() const;
	void createCircleVertices(glm::vec2 const center, float const radius);
	void freeVAO();

private:
	unsigned int m_samplePointsNo;
	GLuint m_vao;
	GLuint m_vbo;
	std::vector<float> m_circleVertices;
};

#endif