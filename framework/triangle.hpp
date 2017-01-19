#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <glm/vec2.hpp>
#include <memory>
#include "line.hpp"

class Triangle
{
public:
	Triangle();
	Triangle(const glm::vec2 p1, const glm::vec2 p2, const glm::vec2 p3);
	~Triangle();
	const bool circumcircleContains(const glm::vec2 &v);
	const bool containsVertex(const glm::vec2 &v);
	const void setTLeft(std::shared_ptr<Triangle> tleft);
	const void setTRight(std::shared_ptr<Triangle> tright);
	const void setTUp(std::shared_ptr<Triangle> tup);
	const bool equals(Triangle other_triangle);

	glm::vec2 m_p1;
	glm::vec2 m_p2;
	glm::vec2 m_p3;
	Line m_l1;
	Line m_l2;
	Line m_l3;

	std::shared_ptr<Triangle> m_tleft;
	std::shared_ptr<Triangle> m_tright;
	std::shared_ptr<Triangle> m_tup;

};

#endif // !TRIANGLE_HPP
