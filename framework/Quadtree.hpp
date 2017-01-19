#ifndef QUADTREE_HPP
#define QUADTREE_HPP

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
#include "Rectangle.hpp"
#include <array>
#include "node.hpp"
#include <memory>
#include "Delaunay.hpp"

struct AABB
{
    glm::vec2 lowerCorner;
    glm::vec2 upperCorner;
	glm::vec2 center;
	glm::vec2 halfSize;

    AABB(glm::vec2 lower = glm::vec2(), glm::vec2 upper = glm::vec2()): lowerCorner(lower), upperCorner(upper)
	{
		halfSize = glm::vec2((upperCorner.x - lowerCorner.x) / 2, (upperCorner.y - lowerCorner.y));
		center = glm::vec2(lowerCorner.x + halfSize.x, lowerCorner.y + halfSize.y);
	};

    bool contains(glm::vec2 point)
    {
		/*if(point.x <= upperCorner.x && point.x > lowerCorner.x && point.y < upperCorner.y && point.x > lowerCorner.y)
        {
			return true;
        }
		return false;*/
		if (point.x < center.x + halfSize.x && point.x > center.x - halfSize.x)
		{
			if (point.y < center.y + halfSize.y && point.y > center.y - halfSize.y)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
    }

    bool intersects(AABB other)
    {
        //this right > that left                                          this left <s that right
		if(upperCorner.x > other.lowerCorner.x || lowerCorner.x < other.upperCorner.x)
        {
        // This bottom > that top
			if(upperCorner.y > other.lowerCorner.y || lowerCorner.y < other.upperCorner.y)
            {
                return true;
            }
        }

        return false;
    }
};


class Quadtree
{
public:
  Quadtree();
  Quadtree(const AABB boundary, const unsigned int capacity);
  ~Quadtree();
  bool insert(std::shared_ptr<Node> point);
  void subdivide();
  std::vector<glm::vec2> queryRange(AABB range);
  std::array<std::shared_ptr<Quadtree>, 4> m_children;
  AABB m_boundary;
  std::vector<glm::vec2> m_triangPoints;
  void createTriangPoints();
  void setTriangPointsNo(const int x, const int y);
  std::vector<Line> createTriangulation();

private:
	unsigned int m_capacity;
	std::vector<glm::vec2> m_points;
	int m_pointsNo_x;
	int m_pointsNo_y;
	
};

#endif
