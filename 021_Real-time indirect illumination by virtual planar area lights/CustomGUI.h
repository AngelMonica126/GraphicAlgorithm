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

	glm::vec3 m_LightPos = glm::vec3(-0.15, -1.13, -0.58);	//对应1.0的LightCameraSize，去除红光的话z=0.19
	glm::vec3 m_LightDir = glm::normalize(glm::vec3(-1.0, -0.7071, 0));	//-0.3, -1, 0
	glm::vec3 m_LightColor = glm::vec3(1, 1, 1);
	float m_Intensity = 5.0;
};