#ifndef DELAUNAY_HPP
#define DELAUNAY_HPP

#include "triangle.hpp"
#include "line.hpp"
#include <vector>

class Line;
class Triangle;

class Delaunay
{
public:
	Delaunay();
	~Delaunay();
	void triangulate(const std::vector<glm::vec2> &vertices);
	void draw();
	void addVertex(glm::vec2 vertex);

	std::vector<Triangle> m_triangles;
	std::vector<glm::vec2> m_vertices;
	std::vector<Line> m_edges;
	std::vector<Line> m_edgeBuffer;
};
#endif // !DELAUNAY_HPP
