#include "triangulationModel.hpp"
#include <array>
#include <iostream>
#include <stack>

TriangulationModel::TriangulationModel(float domainWidth, float domainHeight)
{
	lowerLeftCorner = glm::vec2(0.0f, 0.0f);
	m_domainWidth = domainWidth;
	m_domainHeight = domainHeight;
	redrawDomainRectangle();
	m_quadtree = std::make_shared<Quadtree>(AABB(lowerLeftCorner, glm::vec2(lowerLeftCorner.x + m_domainWidth, lowerLeftCorner.y + m_domainHeight)), 4);
}

TriangulationModel::~TriangulationModel()
{
	
}

float TriangulationModel::map(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void TriangulationModel::redrawDomainRectangle()
{
	m_domainRectangle.reloadModelData(glm::vec2(0.0f, 0.0f), glm::vec2(m_domainWidth, m_domainHeight));
	m_quadtree = std::make_shared<Quadtree>(AABB(glm::vec2(0.0f, 0.0f), glm::vec2(m_domainWidth, m_domainHeight)), 1);
	for (const std::shared_ptr<CrackDomain> crack: m_crackDomains)
	{
		for (std::shared_ptr<Node> point : crack->m_points)
		{
			m_quadtree->insert(point);
		}
	}
}

void TriangulationModel::drawDomainRectangle()
{
	m_domainRectangle.draw();
}

void TriangulationModel::addCrackDomain(glm::vec2 const startPoint, glm::vec2 const endPoint, unsigned int const samplePointsNo)
{
	std::shared_ptr<CrackDomain> newCrack = std::make_shared<CrackDomain>(glm::vec2(startPoint.x, startPoint.y), 
		glm::vec2(endPoint.x, endPoint.y), 45.0f, 2, samplePointsNo);
	m_crackDomains.push_back(newCrack);
	//int ctr = 0;
	for (std::shared_ptr<Node> point : newCrack->m_points)
	{
		//if (ctr < 5) 
		m_quadtree->insert(point);
		//ctr++;
	}
	quadsToDraw.clear();
	visitQuad(m_quadtree);
	/*for (std::vector<Line> triangVect: m_triangLines)
	{
		for (Line triangLine : triangVect)
		{
			std::cout << "\n triang line " << triangLine.min.x << " " << triangLine.min.y << " " << triangLine.max.x << " " << triangLine.max.y;
		}
	}*/
}

void TriangulationModel::drawCracks()
{
	//std::cout<<"drawing cracks "<<m_crackDomains.size()<<"\n";
	for (unsigned int index = 0; index < m_crackDomains.size(); ++index)
	{
		//std::cout<<"drawing crack domain \n";
		m_crackDomains.at(index)->drawCrackDomain();
	}
}

void TriangulationModel::visitQuad(std::shared_ptr<Quadtree> quad)
{
	int index = 0;
	if (quad != nullptr)
	{
		std::stack<std::shared_ptr<Quadtree>> visitedQuads;
		visitedQuads.push(quad);

		while (!visitedQuads.empty())
		{
			std::shared_ptr<Quadtree> topNode = visitedQuads.top();
			visitedQuads.pop();
			topNode->setTriangPointsNo(2, 2);
			if (topNode->m_children[0] != nullptr)
			{
				for (auto child : topNode->m_children)
				{
					visitedQuads.push(child);
				}
			}
			if (topNode != quad)
			{
				index++;
				std::shared_ptr<Rectangle> quad = std::make_shared<Rectangle>(topNode->m_boundary.lowerCorner, topNode->m_boundary.upperCorner);
				quadsToDraw.push_back(quad);
				if (index == 4)
				{
					auto edges = topNode->createTriangulation();
					m_triangLines.push_back(edges);
					m_triangPoints.push_back(topNode->m_triangPoints);
				}
			}

		}
	}
}


void TriangulationModel::drawQuads()
{
	for (const std::shared_ptr<Rectangle> quad : quadsToDraw)
	{
		quad->draw();
	}
}

void TriangulationModel::drawTriangles()
{
	if (m_triangLines.size() > 0)
	{
		for (int i = 0; i < 1; i++)
		{
			for (auto j = m_triangLines[i].begin(); j != m_triangLines[i].end(); ++j)
			{
				(*j).draw();
			}
		}
	}
	/*for (auto i = m_triangLines.begin(); i != m_triangLines.end(); ++i)
	{
		for (auto j = (*i).begin(); j != (*i).end(); ++j)
		{
			(*j).draw();
		}
	}*/
}

void TriangulationModel::drawPoints()
{
	for (const std::shared_ptr<CrackDomain> crack: m_crackDomains)
	{
		//int ctr = 0;
		for (std::shared_ptr<Node> point : crack->m_points)
		{
			Circle curr_circle = Circle(point->getCoordinates(), 0.005);
			//if (ctr < 5) 
			curr_circle.draw();
			//ctr++;
		}
	}
}

void TriangulationModel::drawTriangulationPoints()
{
	for (auto i = m_triangPoints.begin(); i != m_triangPoints.end(); ++i)
	{
		for (auto j = (*i).begin(); j != (*i).end(); ++j)
		{
			Circle curr = Circle((*j), 0.005);
			curr.draw();
		}
	}
}

/*void TriangulationDomain::setRadius(float &radius)
{
    _radius = radius;
}

void TriangulationDomain::setCenter(glm::vec2 &center)
{
    _center = center;
}

void TriangulationDomain::setSamplePointsNumber(unsigned int &samplePointsNo)
{
	_samplePointsNo = samplePointsNo;
}

void TriangulationDomain::createVertices(void)
{
	std::cout<<"creating vertices \n";
	for (int index = 0; index < _samplePointsNo; index++)   
	{
		float theta = 2.0f * pi * float(index) / float(_samplePointsNo); //get the current angle 
        float x = _radius * cosf(theta); //calculate the x component 
        float y = _radius * sinf(theta); //calculate the y component 
		//TriangulationDomainVertices.push_back(x + _center.x);
		//TriangulationDomainVertices.push_back(y + _center.y);
    }
	//std::for_each(TriangulationDomainVertices.begin(), TriangulationDomainVertices.end(), [](float x){std::cout << "\n TriangulationDomain vertex " << x;});
}

void TriangulationDomain::draw() const
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


void TriangulationDomain::freeVAO()
{
    glDeleteVertexArrays(1, &m_vao);
}
*/
