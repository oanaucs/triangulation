#include "node.hpp"
#include <array>
#include <iostream>

Node::Node() : m_number(0), m_coordinates(glm::vec2(0,0))
{

}

Node::Node(int const number, glm::vec2 const coordinates)
	: m_number(number), m_coordinates(coordinates)
{

}

Node::~Node()
{

}

int Node::getNumber()
{
	return m_number;
}

glm::vec2 Node::getCoordinates()
{
	return m_coordinates;
}

void Node::setNumber(int const number)
{
	m_number = number;
}

void Node::setCoordinates(glm::vec2 const coordinates)
{
	m_coordinates = coordinates;
}