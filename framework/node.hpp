#ifndef NODE_HPP
#define NODE_HPP
#include <glm/vec2.hpp>
#include <vector>

class Node
{
public:
	Node();
	Node(int const number, glm::vec2 const coordinates);
	~Node();
	int getNumber();
	glm::vec2 getCoordinates();
	void setNumber(int const number);
	void setCoordinates(glm::vec2 const coordinates);

private:
	int m_number;
	glm::vec2 m_coordinates;
};

#endif