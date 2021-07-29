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

	glm::vec3 m_LightPos = glm::vec3(0, -0.304485, 2.43392);
	glm::vec3 m_LightDir = glm::normalize(glm::vec3(1, -1, 0));
	float m_Intensity = 10.0f;
};