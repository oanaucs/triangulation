#include "Delaunay.hpp"
#include <math.h>
#include <algorithm>
#include <iostream>
#include <glm/geometric.hpp>

Delaunay::Delaunay() {}

Delaunay::~Delaunay() {}

void Delaunay::addVertex(glm::vec2 vertex)
{
	//std::cout << "\n triangles size " << m_triangles.size() << "\n";
	//m_edgeBuffer.clear();

	std::vector<Triangle> badTriangles;

	for (auto triang_it = m_triangles.begin(); triang_it != m_triangles.end(); ++triang_it) {
		if ((*triang_it).circumcircleContains(vertex))
		{
			badTriangles.push_back(*triang_it);
		}
	}

	std::vector<Line> polygon;

	for (auto i = badTriangles.begin(); i != badTriangles.end(); ++i)
	{
		bool l1_shared = false;
		bool l2_shared = false;
		bool l3_shared = false;
		for (auto j = badTriangles.begin(); j != badTriangles.end(); ++j)
		{
			if (i != j)
			{
				if ((*j).m_l1.equals((*i).m_l1) || (*j).m_l1.equals((*i).m_l2) || (*j).m_l1.equals((*i).m_l3))
				{
					l1_shared = true;
				}

				if ((*j).m_l2.equals((*i).m_l1) || (*j).m_l2.equals((*i).m_l2) || (*j).m_l2.equals((*i).m_l3))
				{
					l2_shared = true;
				}

				if ((*j).m_l3.equals((*i).m_l1) || (*j).m_l3.equals((*i).m_l2) || (*j).m_l3.equals((*i).m_l3))
				{
					l3_shared = true;
				}
			}
		}
		if (l1_shared == false) polygon.push_back((*i).m_l1);
		if (l2_shared == false) polygon.push_back((*i).m_l2);
		if (l3_shared == false) polygon.push_back((*i).m_l3);
	}

		auto k = m_triangles.begin();
		for (; k != m_triangles.end();)
		{
			bool equals = false;
			for (auto j = badTriangles.begin(); j != badTriangles.end(); ++j)
			{
				if ((*k).equals((*j))) equals = true;
			}
			if (equals) k = m_triangles.erase(k);
			else ++k;
		}

		for (auto i = polygon.begin(); i != polygon.end(); ++i)
		{
			Triangle newTriangle = Triangle((*i).m_min, (*i).m_max, vertex);
			m_triangles.push_back(newTriangle);
		}
	
}


	/*auto triang_it = m_triangles.begin();
	for (; triang_it != m_triangles.end(); ) {
		if ((*triang_it).circumcircleContains(vertex))
		{
			m_edgeBuffer.push_back((*triang_it).m_l1);
			m_edgeBuffer.push_back((*triang_it).m_l2);
			m_edgeBuffer.push_back((*triang_it).m_l3);
			
			triang_it = m_triangles.erase(triang_it);
		}
		else {
			++triang_it;
		}
	}*/

	
	/*for (auto i = m_edgeBuffer.begin(); i != m_edgeBuffer.end(); ++i)
	{
		auto new_it = m_edgeBuffer.begin();
		for (; new_it != m_edgeBuffer.end();)
		{
			bool equal = false;
			if ((*i).m_min == (*new_it).m_min && (*i).m_max == (*new_it).m_max) equal = true;
			if ((*i).m_min == (*new_it).m_max && (*i).m_max == (*new_it).m_min) equal = true;
			if (i != new_it && equal)
			{
				new_it = m_edgeBuffer.erase(new_it);
			}
			else
			{
				++new_it;
			}
		}
	}

	for (Line edge : m_edgeBuffer)
	{
		m_triangles.push_back(Triangle(edge.m_min, edge.m_max, vertex));
	}
}*/


void Delaunay::triangulate(const std::vector<glm::vec2> &vertices)
{
	m_vertices = vertices;
	// create super triangle
	float min_x = m_vertices[0].x;
	float min_y = m_vertices[0].y;

	float max_x = min_x;
	float max_y = min_y;

	for (glm::vec2 vertex : m_vertices)
	{
		if (vertex.x < min_x) min_x = vertex.x;
		if (vertex.y < min_y) min_y = vertex.y;

		if (vertex.x > max_x) max_x = vertex.x;
		if (vertex.y > max_y) max_y = vertex.y;
	}

	float delta_x = max_x - min_x;
	float delta_y = max_y - min_y;
	float delta_max = std::max(delta_y, delta_y);
	float mid_x = (max_x + min_x) / 2.0f;
	float mid_y = (max_y + min_y) / 2.0f;


	glm::vec2 p1(mid_x -  5 * delta_max, mid_y - delta_max);
	glm::vec2 p2(mid_x, mid_y +  5 * delta_max);
	glm::vec2 p3(mid_x + 5 * delta_max, mid_y - 5 * delta_max);

	//std::cout << "\n super triangle " << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << " " << p3.x << " " << p3.y << "\n ";
	Triangle triangle = Triangle(p1, p2, p3);
	//std::cout << "\n curr triangle " << triangle.m_l1.m_min.x << '\n';
	m_triangles.clear();
	m_triangles.push_back(triangle);


	/*for (glm::vec2 vertex : vertices)
	{
		std::cout << "\n " << vertex.x << " " << vertex.y;
	}*/

	for (unsigned int i = 0; i < m_vertices.size(); ++i)
	{
		addVertex(m_vertices[i]);
		std::cout << "\n adding vertex " << m_vertices[i].x << " " << m_vertices[i].y << "\n";
	}

	/*for (Line edge: m_edges)
	{
		std::cout << "\n " << edge.min.x << " " << edge.min.y << " " << edge.max.x << " " << edge.max.y;
	}*/

	
	auto i = m_triangles.begin();
	for (; i != m_triangles.end(); )
	{
		bool remove = false;
		if ((*i).containsVertex(p1) || (*i).containsVertex(p2) || (*i).containsVertex(p3)) remove = true;

		if (remove) i = m_triangles.erase(i);
		else ++i;
	}

	std::cout << "\n triangles size " << m_triangles.size();

	m_edges.clear();
	/*for (Triangle triangle : m_triangles)
	{
		m_edges.push_back(triangle.m_l1);
		m_edges.push_back(triangle.m_l2);
		m_edges.push_back(triangle.m_l3);
	}*/
	for (unsigned int i = 0; i < m_triangles.size(); ++i)
	{
		m_edges.push_back(m_triangles[i].m_l1);
		m_edges.push_back(m_triangles[i].m_l2);
		m_edges.push_back(m_triangles[i].m_l3);
	}
}

/*void Delaunay::triangulate(const std::vector<glm::vec2> &vertices)
{
	// Store the vertices localy
	m_vertices = vertices;

	// Determinate the super triangle
	float minX = m_vertices[0].x;
	float minY = m_vertices[0].y;
	float maxX = minX;
	float maxY = minY;

	for (std::size_t i = 0; i < vertices.size(); ++i)
	{
		if (m_vertices[i].x < minX) minX = m_vertices[i].x;
		if (m_vertices[i].y < minY) minY = m_vertices[i].y;
		if (m_vertices[i].x > maxX) maxX = m_vertices[i].x;
		if (m_vertices[i].y > maxY) maxY = m_vertices[i].y;
	}

	float dx = maxX - minX;
	float dy = maxY - minY;
	float deltaMax = std::max(dx, dy);
	float midx = (minX + maxX) / 2.f;
	float midy = (minY + maxY) / 2.f;

	glm::vec2 p1(midx - 20 * deltaMax, midy - deltaMax);
	glm::vec2 p2(midx, midy + 20 * deltaMax);
	glm::vec2 p3(midx + 20 * deltaMax, midy - deltaMax);


	//std::cout << "Super triangle " << std::endl << Triangle(p1, p2, p3) << std::endl;

	// Create a list of triangles, and add the supertriangle in it
	//m_triangles.push_back(Triangle(m_vertices[0], m_vertices[m_vertices.size() / 2], m_vertices[m_vertices.size() - 1]));
	m_triangles.push_back(Triangle(p1, p2, p3));

	for (auto p = begin(m_vertices); p != end(m_vertices); p++)
	{
		//std::cout << "Traitement du point " << *p << std::endl;
		std::cout << "_triangles contains " << m_triangles.size() << " elements" << std::endl;	

		std::vector<Triangle> badTriangles;
		std::vector<Line> polygon;

		for (auto t = begin(m_triangles); t != end(m_triangles); t++)
		{
			//std::cout << "Processing " << std::endl;

			if (t->circumcircleContains(*p))
			{
				std::cout << "Pushing bad triangle " << std::endl;
				badTriangles.push_back(*t);
				polygon.push_back(t->m_l1);
				polygon.push_back(t->m_l2);
				polygon.push_back(t->m_l3);
			}
			else
			{
				std::cout << " does not contain triangle" << " in his circum center" << std::endl;
			}
		}

		m_triangles.erase(std::remove_if(begin(m_triangles), end(m_triangles), [badTriangles](Triangle &t) {
			for (auto bt = begin(badTriangles); bt != end(badTriangles); bt++)
			{
				if (bt->m_p1 == t.m_p1 && bt->m_p2 == t.m_p2 && bt->m_p3 == t.m_p3)
				{
					std::cout << "Removing bad triangle " << std::endl;
					return true;
				}
			}
			return false;
		}), end(m_triangles));

		std::vector<Line> badEdges;
		for (auto e1 = begin(polygon); e1 != end(polygon); e1++)
		{
			for (auto e2 = begin(polygon); e2 != end(polygon); e2++)
			{
				if (e1 == e2)
					continue;

				if (*e1 == *e2)
				{
					badEdges.push_back(*e1);
					badEdges.push_back(*e2);
				}
			}
		}

		polygon.erase(std::remove_if(begin(polygon), end(polygon), [badEdges](Line &e) {
			for (auto it = begin(badEdges); it != end(badEdges); it++)
			{
				if (*it == e)
					return true;
			}
			return false;
		}), end(polygon));

		for (auto e = begin(polygon); e != end(polygon); e++)
			m_triangles.push_back(Triangle(e->min, e->max, *p));

	}

	m_triangles.erase(std::remove_if(begin(m_triangles), end(m_triangles), [p1, p2, p3](Triangle &t) {
		return t.containsVertex(p1) || t.containsVertex(p2) || t.containsVertex(p3);
	}), end(m_triangles));

	for (auto t = begin(m_triangles); t != end(m_triangles); t++)
	{
		m_edges.push_back(t->m_l1);
		m_edges.push_back(t->m_l2);
		m_edges.push_back(t->m_l3);
	}

}*/

void Delaunay::draw()
{
	//std::cout << "\n drawing edges " << m_edges.size();
	for (auto i = m_edges.begin(); i != m_edges.end(); ++i)
	{
		(*i).draw();
	}
}