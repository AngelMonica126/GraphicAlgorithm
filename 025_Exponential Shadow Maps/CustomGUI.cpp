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
	ElayGraphics::ResourceManager::registerSharedData("LightIntensity", m_Intensity);
	ElayGraphics::ResourceManager::registerSharedData("LightColor", m_LightColor);
}

void CCustomGUI::updateV()
{
}