#ifndef CRACK_DOMAIN_HPP
#define CRACK_DOMAIN_HPP
#define PI 3.1415926f
#include "circle.hpp"
#include "line.hpp"
#include <memory>
#include <glm/vec2.hpp>
#include <vector>
#include "node.hpp"

class CrackDomain
{
public:
  CrackDomain(glm::vec2 startPoint, glm::vec2 endPoint, float alpha, float l2, unsigned int samplePointsNo);
  void createDomainSamplePoints(void);
  ~CrackDomain();
  void drawCrackDomain(void) const;
  std::vector<std::shared_ptr<Node>> m_points;

private:
	unsigned int m_samplePointsNo;
	glm::vec2 m_startPoint;
	glm::vec2 m_endPoint;
	float m_radius;
	float m_l2;
	float m_l1;
	float m_r;
	float m_h;
	float m_alpha;
	float m_analyticalDomainRadius;
	std::vector<float> m_circleSamplePoints;
	Line m_line;
	Circle m_circle;

};

#endif 
