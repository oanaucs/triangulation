#include "Quadtree.hpp"
#include <array>
#include <iostream>

Quadtree::Quadtree() : m_capacity(4)
{

}

Quadtree::Quadtree(const AABB boundary, const unsigned int capacity): 
	m_boundary(boundary), m_capacity(capacity)
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		m_children[i] = nullptr;
	}
}

Quadtree::~Quadtree()
{

}

bool Quadtree::insert(std::shared_ptr<Node> point)
{
	if (!m_boundary.contains(point->getCoordinates()))
	{
		return false;
		std::cout << "\n does not contain point";
	}
	else
	{
		if (m_points.size() < m_capacity)
		{
			std::cout << "\n adding point";
			m_points.push_back(point->getCoordinates());
			return true;
		}
		else
		{
			std::cout<<"\n subdivision "<<point->getNumber();
			if (m_children[0] == nullptr) subdivide();
			if (m_children[0]->insert(point)) return true;
			if (m_children[1]->insert(point)) return true;
			if (m_children[2]->insert(point)) return true;
			if (m_children[3]->insert(point)) return true;
		}
	}
	return false;

}

void Quadtree::subdivide()
{
	float length_x = m_boundary.upperCorner.x - m_boundary.lowerCorner.x;
	float length_y = m_boundary.upperCorner.y - m_boundary.lowerCorner.y;

	glm::vec2 sw_upper = glm::vec2(length_x/2.0 + m_boundary.lowerCorner.x, 
		length_y/2.0 + m_boundary.lowerCorner.y);
	m_children[1] = std::make_shared<Quadtree>(AABB(m_boundary.lowerCorner, sw_upper), m_capacity);

	glm::vec2 se_lower = glm::vec2(length_x/2.0 + m_boundary.lowerCorner.x, m_boundary.lowerCorner.y);
	glm::vec2 se_upper = glm::vec2(m_boundary.upperCorner.x, length_y/2.0 + m_boundary.lowerCorner.y);
	m_children[2] = std::make_shared<Quadtree>(AABB(se_lower, se_upper), m_capacity);

	glm::vec2 ne_lower = glm::vec2(length_x/2.0 + m_boundary.lowerCorner.x,
		length_y/2.0 + m_boundary.lowerCorner.y);
	m_children[3] = std::make_shared<Quadtree>(AABB(ne_lower, m_boundary.upperCorner), m_capacity);

	glm::vec2 nw_lower = glm::vec2(m_boundary.lowerCorner.x, length_y / 2.0 + m_boundary.lowerCorner.y);
	glm::vec2 nw_upper = glm::vec2(length_x/ 2.0 + m_boundary.lowerCorner.x, m_boundary.upperCorner.y);
	m_children[0] = std::make_shared<Quadtree>(AABB(nw_lower, nw_upper), m_capacity);
}

std::vector<glm::vec2> Quadtree::queryRange(AABB queryRange)
{
	std::vector<glm::vec2> pointsInRange;
	if (!m_boundary.intersects(queryRange)) return pointsInRange;
	
	for (unsigned int index = 0; index < m_points.size(); ++index)
	{
		if (queryRange.contains(m_points.at(index))) pointsInRange.push_back(m_points.at(index));
	}

	if (m_children[0] == nullptr) return pointsInRange;

	std::vector<glm::vec2> range_nw = m_children[0]->queryRange(queryRange);
	pointsInRange.insert(pointsInRange.end(), range_nw.begin(), range_nw.end());
	std::vector<glm::vec2> range_ne = m_children[1]->queryRange(queryRange);
	pointsInRange.insert(pointsInRange.end(), range_ne.begin(), range_ne.end());
	std::vector<glm::vec2> range_sw = m_children[2]->queryRange(queryRange);
	pointsInRange.insert(pointsInRange.end(), range_sw.begin(), range_sw.end());
	std::vector<glm::vec2> range_se = m_children[3]->queryRange(queryRange);
	pointsInRange.insert(pointsInRange.end(), range_se.begin(), range_se.end());

	return pointsInRange;
}

void Quadtree::setTriangPointsNo(const int x, const int y)
{
	m_pointsNo_x = x;
	m_pointsNo_y = y;
}

void Quadtree::createTriangPoints()
{
	float dist_x = m_boundary.upperCorner.x - m_boundary.lowerCorner.x;
	float dist_y = m_boundary.upperCorner.y - m_boundary.lowerCorner.y;
	float step_x = dist_x / (float)m_pointsNo_x;
	float step_y = dist_y / (float)m_pointsNo_y;

	//std::cout << "\n step x " << m_boundary.lowerCorner.x << " step y " << m_boundary.lowerCorner.y;

	for (int j = 0; j <= m_pointsNo_y; ++j)
	{
		for (int i = 0; i <= m_pointsNo_x; ++i)
		{
			glm::vec2 point = glm::vec2(m_boundary.lowerCorner.x + i * step_x, m_boundary.lowerCorner.y + j * step_y);
			m_triangPoints.push_back(point);
		}
	}
}

std::vector<Line> Quadtree::createTriangulation()
{
	createTriangPoints();
	Delaunay triangulation = Delaunay();
	//std::cout << "\n quad triangulation " << m_triangPoints.size() << " vertices \n";
	triangulation.triangulate(m_triangPoints);
	//std::cout << "\n quad triangulation done " << triangulation.m_edgeBuffer.size() << " edges \n";
	return triangulation.m_edges;
}