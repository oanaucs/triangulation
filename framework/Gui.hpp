#ifndef GUI_HPP
#define GUI_HPP
#include "circle.hpp"
#include "triangulationModel.hpp"
#include <imgui.h>
#include <memory>
#include <glm/vec2.hpp>
#include <vector>

class Gui
{
public:
  Gui(TriangulationModel* triangModel);
  ~Gui();
  void showGUIElements();
  void createDomainHeader();
  void createCrackDomainsHeader();
  void createTable();

private:
	TriangulationModel* m_triangModel;
	bool m_addNewCrackButtonClicked;
	float m_newStartCoords[2];
	float m_newEndCoords[2];
	float m_startCoords[2];
	float m_endCoords[2];
	std::vector<std::pair<float, float>> m_scoordinates;
	std::vector<std::pair<float, float>> m_ecoordinates;
};

#endif 
