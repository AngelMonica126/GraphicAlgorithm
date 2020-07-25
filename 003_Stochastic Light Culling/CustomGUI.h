#pragma once
#include "GUI.h"
#include <GLM/glm.hpp>

class CCustomGUI : public IGUI
{
public:
	CCustomGUI(const std::string& vName, int vExcutionOrder);
	virtual ~CCustomGUI();

	virtual void initV() override;
	virtual void updateV() override;

private:
	glm::vec3 m_LightPos = glm::vec3(0, 1, 0);	//30, 308, -130
	glm::vec3 m_LightDir = glm::normalize(glm::vec3(-0.3, -1, 0));
};