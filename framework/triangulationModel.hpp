#ifndef TRIANGULATION_MODEL_HPP
#define TRIANGULATION_MODEL_HPP

#include <vector>
#include <map>
#include "CrackDomain.hpp"
#include <memory>
//#include "Rectangle.hpp"
#include "Quadtree.hpp"

class TriangulationModel
{
public:
	float m_domainWidth;
	float m_domainHeight;
	TriangulationModel(float domainWidth, float domainHeight);
	~TriangulationModel();
	void addCrackDomain(glm::vec2 const startPoint, glm::vec2 const endPoint, unsigned int const samplePointsNo);
	void redrawDomainRectangle();
	float map(float x, float in_min, float in_max, float out_min, float out_max);
	void drawDomainRectangle();
	void drawCracks();
	void drawQuads();
	void drawPoints();
	void drawTriangles();
	void drawTriangulationPoints();
	void visitQuad(std::shared_ptr<Quadtree> quad);
	/*void createVertices(void);
	void setRadius(float &radius);
	void setSamplePointsNumber(unsigned int &samplePointsNo);
	void setCenter(glm::vec2 &center);
	void draw() const;
	void freeVAO();*/

private:
	std::vector<std::shared_ptr<CrackDomain>> m_crackDomains;
	Rectangle m_domainRectangle;
	glm::vec2 lowerLeftCorner;
	std::vector<glm::vec2> m_points;
	std::vector<std::shared_ptr<Rectangle>> quadsToDraw;
	std::shared_ptr<Quadtree> m_quadtree;
	std::vector<std::vector<Line>> m_triangLines;
	std::vector<std::vector<glm::vec2>> m_triangPoints;
};

#endif