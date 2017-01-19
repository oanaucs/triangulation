#include "CrackDomain.hpp"
#include <glm/geometric.hpp>
#include <iostream>
#include <algorithm>

CrackDomain::CrackDomain(glm::vec2 startPoint, glm::vec2 endPoint, float alpha, float l2, unsigned int samplePointsNo):
	m_startPoint(startPoint), m_endPoint(endPoint), m_alpha(alpha), m_l2(l2), m_samplePointsNo(samplePointsNo)
{
	m_radius = glm::length(m_endPoint - m_startPoint)/4.0;
	std::cout <<"\n m_radius: " << m_radius;
	createDomainSamplePoints();
	m_circle = Circle(m_circleSamplePoints);
	//glm::vec2 lineStart = glm::vec2(m_startPoint.x - (m_endPoint.x - m_startPoint.x), m_startPoint.y);
	m_line = Line(m_startPoint, m_endPoint);
	m_l1 = m_radius;
	m_l2 = m_radius * 1/3;
	m_h = std::sqrt(std::pow((m_l1 + m_l2) / std::cos((alpha + 90.0) * PI / 180.0), 2) - std::pow(m_l1 + m_l2, 2));
	std::cout<<"\n m_h "<< m_h;
	glm::vec2 lowerCorner = glm::vec2(endPoint.x - m_radius * 2, endPoint.y - m_h); //bounding square lower point
	glm::vec2 upperCorner = glm::vec2(endPoint.x + m_radius * 2, endPoint.y + m_h); //bounding square upper point
	int noPoints = 2;
	int counter = 0;
	float step_x = (upperCorner.x - lowerCorner.x) / float(noPoints);
	float step_y = (upperCorner.y - lowerCorner.y) / float(noPoints);
	for (unsigned int i = 0; i < noPoints; i++)
	{
		for (unsigned int j = 0; j < noPoints; j++)
		{
			glm::vec2 currPoint = glm::vec2(lowerCorner.x + step_x * i, lowerCorner.y + step_y * j);
			std::shared_ptr<Node> node = std::make_shared<Node>(counter, currPoint);
			m_points.push_back(node);
			counter++;
		}
	}

	//for (unsigned int i = 0; i < noPoints; i++) //do not go until the prev pushed point
	//{
	//	for (unsigned int j = 0; j < noPoints; j++)
	//	{
	//		glm::vec2 currPoint = glm::vec2(upperCorner.x - m_radius * i, upperCorner.y - m_h * j);
	//		std::shared_ptr<Node> node = std::make_shared<Node>(counter, currPoint);
	//		m_points.push_back(node);
	//		counter++;;
	//	}
	//}
}

void CrackDomain::createDomainSamplePoints(void)
{
	for (unsigned int index = 0; index < m_samplePointsNo; index++)   
	{
		float theta = 2.0f * PI * float(index) / float(m_samplePointsNo); //get the current angle 
        float x = m_radius * cosf(theta); //calculate the x component 
        float y = m_radius * sinf(theta); //calculate the y component 
		m_circleSamplePoints.push_back(x + m_endPoint.x);
		m_circleSamplePoints.push_back(y + m_endPoint.y);
		std::cout<<"\n creating vertices "<<x + m_endPoint.x << " " << y + m_endPoint.y;
    }
	//std::for_each(samplePoints.begin(), samplePoints.end(), [](glm::vec2 currPoint){std::cout << "\n circle vertex " << currPoint.x << " " << currPoint.y;});
}

/*CrackDomain::CrackDomain(glm::vec2 crackStartPoint, glm::vec2 crackEndPoint)
{
	crack = new Crack(crackStartPoint, crackEndPoint);
	radius = crack->endPoint.x - crack->startPoint.x;
	analyticalDomainCenter = crack->endPoint;
}*/

/*void Circle::setRadius(float &radius)
{
    m_radius = radius;
}

void Circle::setCenter(glm::vec2 &center)
{
    m_center = center;
}

void Circle::setSamplePointsNumber(unsigned int &samplePointsNo)
{
	m_samplePointsNo = samplePointsNo;
}*/

CrackDomain::~CrackDomain(void)
{

}

void CrackDomain::drawCrackDomain(void) const
{
	m_circle.draw();
	m_line.draw();
}

/*glm::vec2 CrackDomain::getCrackStartPoint(void) const
{
	return crack->startPoint;
}

glm::vec2 CrackDomain::getCrackEndPoint(void) const
{
	return crack->endPoint;
}

float CrackDomain::getRadius(void) const
{
	return radius;
}

glm::vec2 CrackDomain::getAnalyticalDomainCenter(void) const
{
	return analyticalDomainCenter;
}

void CrackDomain::drawCrack(void) const
{

}

void CrackDomain::drawAnalyticalDomain(void) const
{

}*/


