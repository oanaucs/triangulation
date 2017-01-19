#include "triangle.hpp"
#include <math.h>

Triangle::Triangle() {}

Triangle::~Triangle() {}

Triangle::Triangle(const glm::vec2 p1, const glm::vec2 p2, const glm::vec2 p3) :
	m_p1(p1), m_p2(p2), m_p3(p3)
{
	m_l1 = Line(m_p1, m_p2);
	m_l2 = Line(m_p2, m_p3);
	m_l3 = Line(m_p3, m_p1);
	
}

const bool Triangle::circumcircleContains(const glm::vec2 &v)
{
	/*float ab = (m_p1.x * m_p1.x) + (m_p1.y * m_p1.y);
	float cd = (m_p2.x * m_p2.x) + (m_p2.y * m_p2.y);
	float ef = (m_p3.x * m_p3.x) + (m_p3.y * m_p3.y);

	float circum_x = (ab * (m_p3.y - m_p2.y) + cd * (m_p1.y - m_p3.y) + ef * (m_p2.y - m_p1.y)) / (m_p1.x * (m_p3.y - m_p2.y) + m_p2.x * (m_p1.y - m_p3.y) + m_p3.x * (m_p2.y - m_p1.y)) / 2.0f;
	float circum_y = (ab * (m_p3.x - m_p2.x) + cd * (m_p1.x - m_p3.x) + ef * (m_p2.x - m_p1.x)) / (m_p1.y * (m_p3.x - m_p2.x) + m_p2.y * (m_p1.x - m_p3.x) + m_p3.y * (m_p2.x - m_p1.x)) / 2.0f;
	float circum_radius = sqrtf(((m_p1.x - circum_x) * (m_p1.x - circum_x)) + ((m_p1.y - circum_y) * (m_p1.y - circum_y)));

	float dist = sqrtf(((v.x - circum_x) * (v.x - circum_x)) + ((v.y - circum_y) * (v.y - circum_y)));
	return dist < circum_radius;*/
	float d = (m_p1.x - m_p3.x) * (m_p2.y - m_p3.y) - (m_p2.x - m_p3.x) * (m_p1.y - m_p3.y);

	float c_x = (((m_p1.x - m_p3.x) * (m_p1.x + m_p3.x) + (m_p1.y - m_p3.y) * (m_p1.y + m_p3.y)) / 2 * (m_p2.y - m_p3.y)
		- ((m_p2.x - m_p3.x) * (m_p2.x + m_p3.x) + (m_p2.y - m_p3.y) * (m_p2.y + m_p3.y)) / 2 * (m_p1.y - m_p3.y))
		/ d;

	float c_y = (((m_p2.x - m_p3.x) * (m_p2.x + m_p3.x) + (m_p2.y - m_p3.y) * (m_p2.y + m_p3.y)) / 2 * (m_p1.x - m_p3.x)
		- ((m_p1.x - m_p3.x) * (m_p1.x + m_p3.x) + (m_p1.y - m_p3.y) * (m_p1.y + m_p3.y)) / 2 * (m_p2.x - m_p3.x))
		/ d;

	float squared_radius = (m_p3.x - c_x)* (m_p3.x - c_x) + (m_p3.y - c_y) * (m_p3.y - c_y);

	return (v.x - c_x) * (v.x - c_x) + (v.y - c_y) * (v.y - c_y) < squared_radius;
}

const bool Triangle::containsVertex(const glm::vec2 &v)
{
	return (m_p1 == v || m_p2 == v || m_p3 == v);
}

const void Triangle::setTUp(std::shared_ptr<Triangle> tup)
{
	m_tup = tup;
}
const void Triangle::setTRight(std::shared_ptr<Triangle> tright)
{
	m_tright = tright;
}

const void Triangle::setTLeft(std::shared_ptr<Triangle>  tleft)
{
	m_tleft = tleft;
}

const bool Triangle::equals(Triangle other_triangle)
{
	return	(m_p1 == other_triangle.m_p1 || m_p1 == other_triangle.m_p2 || m_p1 == other_triangle.m_p3) &&
		(m_p2 == other_triangle.m_p1 || m_p2 == other_triangle.m_p2 || m_p2 == other_triangle.m_p3) &&
		(m_p3 == other_triangle.m_p1 || m_p3 == other_triangle.m_p2 || m_p3 == other_triangle.m_p3);
}