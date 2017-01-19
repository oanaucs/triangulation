#include "Gui.hpp"
#include <iostream>
#include <string>

Gui::Gui(TriangulationModel* triangModel)
{
	m_triangModel = triangModel;
	m_addNewCrackButtonClicked = false;
	m_newStartCoords[0] = 0.0f;
	m_newStartCoords[1] = 0.0f;
	m_newEndCoords[0] = 0.0f;
	m_newEndCoords[1] = 0.0f;
	m_startCoords[0] = 0.0f;
	m_startCoords[1] = 0.0f;
	m_endCoords[0] = 0.0f;
	m_endCoords[1] = 0.0f;
}

Gui::~Gui()
{

}

void Gui::createDomainHeader()
{
	if (ImGui::CollapsingHeader("Domain Settings", 0, true, true))
    {
		float currentWidthValue = m_triangModel->m_domainWidth;
		ImGui::SliderFloat("Length Value", &m_triangModel->m_domainWidth, 0.0f, 6.0f, "%.8f", 1.0f);
		if (m_triangModel->m_domainWidth != currentWidthValue)
		{
			//load gl model for rectangle
			m_triangModel->redrawDomainRectangle();
		}

		float currentHeightValue = m_triangModel->m_domainHeight;
        ImGui::SliderFloat("Width Value", &m_triangModel->m_domainHeight, 0.0f, 6.0f, "%.8f", 1.0f);
		if (m_triangModel->m_domainHeight != currentHeightValue)
		{
			//load gl model for rectangle
			m_triangModel->redrawDomainRectangle();
		}
    }
}

void Gui::createTable()
{
	ImGui::Text("Added crack domains");
    ImGui::Columns(5, "Crack domains"); // 4-ways, with border
    ImGui::Separator();
    ImGui::Text("ID"); ImGui::NextColumn();
    ImGui::Text("Start X"); ImGui::NextColumn();
    ImGui::Text("Start Y"); ImGui::NextColumn();
    ImGui::Text("End X"); ImGui::NextColumn();
	ImGui::Text("End Y"); ImGui::NextColumn();
    ImGui::Separator();
    static int selected = -1;
	for (int i = 0; i < m_scoordinates.size(); i++)
    {
        char label[32];
        sprintf(label, "%02d", i);
        if (ImGui::Selectable(label, selected == i, ImVec2(0,0)))
            selected = i;
        ImGui::NextColumn();
		ImGui::Text(std::to_string(m_scoordinates.at(i).first).c_str()); ImGui::NextColumn();
		ImGui::Text(std::to_string(m_scoordinates.at(i).second).c_str()); ImGui::NextColumn();
        ImGui::Text(std::to_string(m_ecoordinates.at(i).first).c_str()); ImGui::NextColumn();
        ImGui::Text(std::to_string(m_ecoordinates.at(i).second).c_str()); ImGui::NextColumn();
    }
	ImGui::Columns(1);
	ImGui::Separator();
}

void Gui::createCrackDomainsHeader()
{
	if (ImGui::CollapsingHeader("Add domain crack", 0, true, false))
    {
		ImGui::InputFloat2("Start coordinates", m_startCoords);
				//std::cout<<"\n current coordinates "<<startcoordinates[0]<<" "<<startcoordinates[1];
				
		ImGui::InputFloat2("End coordinates", m_endCoords);
				//std::cout<<"\n current end coordinates "<<endcoordinates[0]<<" "<<endcoordinates[1];
			//m_addNewCrackButtonClicked = false;
		bool add_new = ImGui::Button("Add new");

		if (add_new && m_startCoords[0] > 0.0f && m_startCoords[1] > 0.0f && m_endCoords[0] > 0.0f && m_endCoords[1] > 0.0f)
		{
			m_scoordinates.push_back(std::make_pair(m_startCoords[0], m_startCoords[1]));
			m_ecoordinates.push_back(std::make_pair(m_endCoords[0], m_endCoords[1]));
			m_triangModel->addCrackDomain(glm::vec2(m_startCoords[0], m_startCoords[1]), glm::vec2(m_endCoords[0], m_endCoords[1]), 20);
			add_new = false;
		}
    }
}

void Gui::showGUIElements()
{
	createDomainHeader();
	createCrackDomainsHeader();
	createTable();
}