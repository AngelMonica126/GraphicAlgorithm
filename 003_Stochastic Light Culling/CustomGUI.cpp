#include "CustomGUI.h"
#include "Interface.h"

CCustomGUI::CCustomGUI(const std::string& vName, int vExcutionOrder) : IGUI(vName, vExcutionOrder)
{
}

CCustomGUI::~CCustomGUI()
{
}

void CCustomGUI::initV()
{
	ElayGraphics::ResourceManager::registerSharedData("LightPos", m_LightPos);
	ElayGraphics::ResourceManager::registerSharedData("LightDir", m_LightDir);
}

void CCustomGUI::updateV()
{
#ifdef _DEBUG
	glm::vec3 MinLightPos = glm::vec3(-1000), MaxLightPos = glm::vec3(1000);
	float LightPosDragSpeed = 0.01f;
	if (IGUI::dragScalarN("LightPos", ElayGraphics::EDataType::DataType_Float, &m_LightPos, 3, LightPosDragSpeed, &MinLightPos, &MaxLightPos))
		ElayGraphics::ResourceManager::updateSharedDataByName("LightPos", m_LightPos);
	glm::vec3 MinLightDir = glm::vec3(-1, -1, -1), MaxLightDir = glm::vec3(1, 1, 1);
	float LightDirDragSpeed = 0.01f;
	if (IGUI::dragScalarN("LightDir", ElayGraphics::EDataType::DataType_Float, &m_LightDir, 3, LightDirDragSpeed, &MinLightDir, &MaxLightDir))
		ElayGraphics::ResourceManager::updateSharedDataByName("LightDir", normalize(m_LightDir));
#endif
}