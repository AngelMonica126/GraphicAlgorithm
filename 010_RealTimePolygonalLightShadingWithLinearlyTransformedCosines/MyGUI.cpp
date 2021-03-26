#include "MyGUI.h"
#include "Interface.h"
#include "Common.h"

CMyGUI::CMyGUI(const std::string& vName, int vExcutionOrder) : IGUI(vName, vExcutionOrder)
{
}

CMyGUI::~CMyGUI()
{
}

//************************************************************************************
//Function:
void CMyGUI::initV()
{
	ElayGraphics::ResourceManager::registerSharedData("DiffuseColor", m_DiffuseColor);
	ElayGraphics::ResourceManager::registerSharedData("SpecularColor", m_SpecularColor);
	ElayGraphics::ResourceManager::registerSharedData("Roughness", m_Roughness);
	ElayGraphics::ResourceManager::registerSharedData("Intensity", m_Intensity);
	ElayGraphics::ResourceManager::registerSharedData("IsTwoSide", m_IsTwoSide);
	ElayGraphics::ResourceManager::registerSharedData("Rotation", m_LightSourceRotation);
}

//************************************************************************************
//Function:
void CMyGUI::updateV()
{
	if (IGUI::colorEdit("Diffuse Color", m_DiffuseColor))
		ElayGraphics::ResourceManager::updateSharedDataByName("DiffuseColor", m_DiffuseColor);
	if (IGUI::colorEdit("Specular Color", m_SpecularColor))
		ElayGraphics::ResourceManager::updateSharedDataByName("SpecularColor", m_SpecularColor);
	float MinRoughness = 0.0f, MaxRoughness = 1.0f;
	if (IGUI::sliderScalar("Roughness", ElayGraphics::EDataType::DataType_Float, &m_Roughness, &MinRoughness, &MaxRoughness))
		ElayGraphics::ResourceManager::updateSharedDataByName("Roughness", m_Roughness);
	float MinIntensity = 0.0f, MaxIntensity = 10.0f;
	if (IGUI::sliderScalar("Intensity", ElayGraphics::EDataType::DataType_Float, &m_Intensity, &MinIntensity, &MaxIntensity))
		ElayGraphics::ResourceManager::updateSharedDataByName("Intensity", m_Intensity);
	float MinRotation = 0.0f, MaxRotation = 360.0f;
	if (IGUI::sliderScalar("Rotation X", ElayGraphics::EDataType::DataType_Float, &m_LightSourceRotation.x, &MinRotation, &MaxRotation))
		ElayGraphics::ResourceManager::updateSharedDataByName("Rotation", m_LightSourceRotation);
	if(IGUI::sliderScalar("Rotation Y",ElayGraphics::EDataType::DataType_Float,&m_LightSourceRotation.y,&MinRotation,&MaxRotation))
		ElayGraphics::ResourceManager::updateSharedDataByName("Rotation", m_LightSourceRotation);
	if (IGUI::sliderScalar("Rotation Z", ElayGraphics::EDataType::DataType_Float, &m_LightSourceRotation.z, &MinRotation, &MaxRotation))
		ElayGraphics::ResourceManager::updateSharedDataByName("Rotation", m_LightSourceRotation);

	if (IGUI::checkBox("IsTwoSide", m_IsTwoSide))
		ElayGraphics::ResourceManager::updateSharedDataByName("IsTwoSide", m_IsTwoSide);
}