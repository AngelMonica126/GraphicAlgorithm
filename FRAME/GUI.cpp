//--------------------------------------------------------------------------------------
// Copyright (c) Elay Pu. All rights reserved.
//--------------------------------------------------------------------------------------

#include "GUI.h"
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include "GLFWWindow.h"
#include "App.h"
#include "ResourceManager.h"
#include "MainGUI.h"
//using namespace ElayGraphics;

IGUI::IGUI()
{
}

IGUI::IGUI(const std::string& vName, int vExcutionOrder) : m_Name(vName), m_ExecutionOrder(vExcutionOrder)
{
}

IGUI::~IGUI()
{
}

//************************************************************************************
//Function:
bool IGUI::operator <(const IGUI& vOtherGUI) const
{
	return m_ExecutionOrder < vOtherGUI.getExecutionOrder();
}

//************************************************************************************
//Function:
const std::shared_ptr<ImFont>& IGUI::getFontByName(const std::string& vFontName)
{
	_ASSERT(m_FontMap.find(vFontName) != m_FontMap.end());
	return m_FontMap[vFontName];
}

//************************************************************************************
//Function:
void IGUI::loadFont(const std::string& vFontFilePath, float vFontSize)
{
	ImFontConfig IconsConfig;
	IconsConfig.MergeMode = true;
	auto pIO = CResourceManager::getOrCreateInstance()->getOrCreateMainGUI()->getIO();
	auto pFont = std::shared_ptr<ImFont>(pIO->Fonts->AddFontFromFileTTF(vFontFilePath.c_str(), vFontSize));
	_ASSERT(pFont);
	pIO->Fonts->AddFontFromFileTTF("../Fonts/fontawesome-webfont.ttf", 16.0f, &IconsConfig, m_IconRanges.data());

	std::vector<std::string> SplitedStringSet;
	boost::split(SplitedStringSet, vFontFilePath, boost::is_any_of("/\\"));
	std::string FontName = *(SplitedStringSet.end() - 1);

	m_FontMap[FontName] = pFont;
}

//************************************************************************************
//Function:
void IGUI::pushFont(const std::shared_ptr<ImFont>& vFont)
{
	ImGui::PushFont(vFont.get());
}

//************************************************************************************
//Function:
void IGUI::popFont()
{
	ImGui::PopFont();
}

//************************************************************************************
//Function:
bool IGUI::combo(const std::string& vLabel, int &vioCurrentItem, const std::vector<std::string>& vItems)
{
	std::vector<const char*> Items;
	for (const auto &e : vItems)
		Items.push_back(e.c_str());
	return ImGui::Combo(vLabel.c_str(), &vioCurrentItem, Items.data(), static_cast<int>(vItems.size()));
}

//************************************************************************************
//Function:
void IGUI::text(const std::string& vText)
{
	ImGui::Text(vText.c_str());
}

//************************************************************************************
//Function:
void IGUI::textDisabled(const std::string& vText)
{
	ImGui::TextDisabled(vText.c_str());
}

//************************************************************************************
//Function:
void IGUI::textColored(const glm::vec4& vColor, const std::string& vText)
{
	ImGui::TextColored(ImVec4(vColor[0], vColor[1], vColor[2], vColor[3]), vText.c_str());
}

//************************************************************************************
//Function:
void IGUI::bulletText(const std::string& vText)
{
	ImGui::BulletText(vText.c_str());
}

//************************************************************************************
//Function:
bool IGUI::beginMenuBar()
{
	return ImGui::BeginMenuBar();
}

//************************************************************************************
//Function:
void IGUI::endMenuBar()
{
	ImGui::EndMenuBar();
}

//************************************************************************************
//Function:
bool IGUI::beginMenu(const std::string& vLable)
{
	return ImGui::BeginMenu(vLable.c_str());
}

//************************************************************************************
//Function:
void IGUI::endMenu()
{
	ImGui::EndMenu();
}

//************************************************************************************
//Function:
bool IGUI::menuItem(const std::string& vLabel)
{
	return ImGui::MenuItem(vLabel.c_str());
}

//************************************************************************************
//Function:
bool IGUI::checkBox(const std::string& vLable, bool &voCheckValue)
{
	return ImGui::Checkbox(vLable.c_str(), &voCheckValue);
}

//************************************************************************************
//Function:
void IGUI::verticalSpacing()
{
	ImGui::Spacing();
}

//************************************************************************************
//Function:
bool IGUI::sliderScalar(const std::string& vLable, ElayGraphics::EDataType vDataType, void* vValue, const void* vMin, const void* vMax)
{
	return ImGui::SliderScalar(vLable.c_str(), static_cast<int>(vDataType), vValue, vMin, vMax);
}

//************************************************************************************
//Function:
bool IGUI::sliderScalarN(const std::string& vLable, ElayGraphics::EDataType vDataType, int vComponentsNum, void* vValue, const void* vMin, const void* vMax)
{
	return ImGui::SliderScalarN(vLable.c_str(), static_cast<int>(vDataType), vValue, vComponentsNum, vMin, vMax);
}


//************************************************************************************
//Function:
bool IGUI::sliderFloat4(const std::string& vLable, glm::vec4& vioValue, float vMin, float vMax)
{
	float TmpData[4];
	return ImGui::SliderFloat4(vLable.c_str(), TmpData, vMin, vMax);
	vioValue = glm::vec4(TmpData[0], TmpData[1], TmpData[2], TmpData[3]);
}

//************************************************************************************
//Function:
bool IGUI::verticalSilderScalar(const std::string& vLable, const glm::vec2& vSliderSize, ElayGraphics::EDataType vDataType, void* vValue, const void* vMin, const void* vMax)
{
	return ImGui::VSliderScalar(vLable.c_str(), ImVec2(vSliderSize[0], vSliderSize[1]), static_cast<int>(vDataType), vValue, vMin, vMax);
}

//************************************************************************************
//Function:
void IGUI::horizontalLine()
{
	ImGui::Separator();
}

//************************************************************************************
//Function:
void IGUI::sameLine()
{
	ImGui::SameLine();
}

//************************************************************************************
//Function:
bool IGUI::button(const std::string& vLable, const glm::vec2& vSize /* = glm::vec2(0) */)
{
	return ImGui::Button(vLable.c_str(), ImVec2(vSize[0], vSize[1]));
}

//************************************************************************************
//Function:
bool IGUI::colorButton(const std::string& vLable, const glm::vec4& vColor, const glm::vec2& vSize)
{
	return ImGui::ColorButton(vLable.c_str(), ImVec4(vColor[0], vColor[1], vColor[2], vColor[3]), 0, ImVec2(vSize[0], vSize[1]));
}

//************************************************************************************
//Function:
void IGUI::setColorPickerType(int vPickerType)
{
	vPickerType ? ImGui::SetColorEditOptions(ImGuiColorEditFlags_PickerHueWheel) : ImGui::SetColorEditOptions(ImGuiColorEditFlags_PickerHueBar);
}

//************************************************************************************
//Function:
bool IGUI::colorEdit(const std::string& vLable, glm::vec4& voColor, bool vIsEditAlpha /* = true */)
{
	ImGuiColorEditFlags Flags = 0;
	if (!vIsEditAlpha)
		Flags |= ImGuiColorEditFlags_NoAlpha;
	return ImGui::ColorEdit4(vLable.c_str(), &voColor[0], Flags);
}

//************************************************************************************
//Function:
bool IGUI::collapsingHeader(const std::string& vLabel)
{
	return ImGui::CollapsingHeader(vLabel.c_str());
}

//************************************************************************************
//Function:
bool IGUI::treeNode(const std::string& vLable)
{
	return ImGui::TreeNode(vLable.c_str());
}

//************************************************************************************
//Function:
void IGUI::treePop()
{
	ImGui::TreePop();
}

//************************************************************************************
//Function:
void IGUI::bullet()
{
	ImGui::Bullet();
}

//************************************************************************************
//Function:
void IGUI::indent()
{
	ImGui::Indent();
}

//************************************************************************************
//Function:
void IGUI::unIndent()
{
	ImGui::Unindent();
}

//************************************************************************************
//Function:
bool IGUI::isLastItemHovered()
{
	return ImGui::IsItemHovered();
}

//************************************************************************************
//Function:
void IGUI::beginTooltip()
{
	return ImGui::BeginTooltip();
}

//************************************************************************************
//Function:
void IGUI::endTooltip()
{
	ImGui::EndTooltip();
}

//************************************************************************************
//Function:
void IGUI::promptMarker(const std::string& vPromptMarker, const std::string& vPromptText)
{
	ImGui::Text(vPromptMarker.c_str());
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text(vPromptText.c_str());
		ImGui::EndTooltip();
	}
}

//************************************************************************************
//Function:
bool IGUI::radioButton(const std::string& vLabel, int &voValue, int vButtonIndex)
{
	return ImGui::RadioButton(vLabel.c_str(), &voValue, vButtonIndex);
}

//************************************************************************************
//Function:
void IGUI::plotLines(const std::string& vLabel, const std::vector<float> &vValueSet, float vMin, float vMax, const glm::vec2& vSize/* = glm::vec2(0)*/)
{
	ImGui::PlotLines(vLabel.c_str(), vValueSet.data(), static_cast<int>(vValueSet.size()), 0, nullptr, vMin, vMax, ImVec2(vSize[0], vSize[1]));
}

//************************************************************************************
//Function:
void IGUI::plotLines(const std::string& vLabel, float(*Func)(void*, int), int vSampleCount, float vMin, float vMax, const glm::vec2& vSize/* = glm::vec2(0)*/)
{
	ImGui::PlotLines(vLabel.c_str(), Func, nullptr, vSampleCount, 0, nullptr, vMin, vMax, ImVec2(vSize[0], vSize[1]));
}

//************************************************************************************
//Function:
void IGUI::plotHistogram(const std::string& vLable, const std::vector<float> &vValueSet, float vMin/* = MAX_VALUE*/, float vMax/* = MAX_VALUE*/, const glm::vec2& vSize/* = glm::vec2(0)*/)
{
	ImGui::PlotHistogram(vLable.c_str(), vValueSet.data(), static_cast<int>(vValueSet.size()), 0, nullptr, vMin, vMax, ImVec2(vSize[0], vSize[1]));
}

//************************************************************************************
//Function:
void IGUI::plotHistogram(const std::string& vLabel, float(*Func)(void*, int), int vSampleCount, float vMin, float vMax, const glm::vec2& vSize/* = glm::vec2(0)*/)
{
	ImGui::PlotHistogram(vLabel.c_str(), Func, nullptr, vSampleCount, 0, nullptr, vMin, vMax, ImVec2(vSize[0], vSize[1]));
}

//************************************************************************************
//Function:
void IGUI::processBar(float vProcessValue, const glm::vec2& vSize/* = glm::vec2(0, 1)*/)
{
	ImGui::ProgressBar(vProcessValue, ImVec2(vSize[0], vSize[1]));
}

//************************************************************************************
//Function:
bool IGUI::inputText(const std::string& vLabel, std::string& voValue, size_t vValueBufferSize/* = 128*/, bool vIsPassword/* = false*/)
{
	voValue.resize(vValueBufferSize);
	ImGuiInputTextFlags Flags = 0;
	if (vIsPassword)
		Flags |= ImGuiInputTextFlags_Password;
	return ImGui::InputText(vLabel.c_str(), &voValue[0], vValueBufferSize, Flags);
}

//************************************************************************************
//Function:
bool IGUI::inputInt(const std::string& vLabel, int &voValue)
{
	return ImGui::InputInt(vLabel.c_str(), &voValue);
}

//************************************************************************************
//Function:
bool IGUI::inputFloat(const std::string& vLabel, float &voValue)
{
	return ImGui::InputFloat(vLabel.c_str(), &voValue);
}

//************************************************************************************
//Function:
bool IGUI::inputDouble(const std::string& vLabel, double &voValue)
{
	return ImGui::InputDouble(vLabel.c_str(), &voValue);
}

//************************************************************************************
//Function:
bool IGUI::inputFloat3(const std::string& vLabel, glm::vec3& voValue)
{
	return ImGui::InputFloat3(vLabel.c_str(), &voValue[0]);
}

//************************************************************************************
//Function:
bool IGUI::inputFloat4(const std::string& vLabel, glm::vec4& voValue)
{
	return ImGui::InputFloat4(vLabel.c_str(), &voValue[0]);
}

//************************************************************************************
//Function:
void IGUI::pushItemWidth(float vItemWidth)
{
	ImGui::PushItemWidth(vItemWidth);
}

//************************************************************************************
//Function:
void IGUI::popItemWidth()
{
	ImGui::PopItemWidth();
}

//************************************************************************************
//Function:
bool IGUI::dragScalar(const std::string& vLabel, ElayGraphics::EDataType vDataType, void* vValue, float vDragSpeed, const void* vMin, const void* vMax)
{
	return ImGui::DragScalar(vLabel.c_str(), static_cast<int>(vDataType), vValue, vDragSpeed, vMin, vMax);
}

//************************************************************************************
//Function:
bool IGUI::dragScalarN(const std::string& vLabel, ElayGraphics::EDataType vDataType, void* vValue, int vComponentsNum, float vDragSpeed, const void* vMin, const void* vMax)
{
	return ImGui::DragScalarN(vLabel.c_str(), static_cast<int>(vDataType), vValue, vComponentsNum, vDragSpeed, vMin, vMax);
}

//************************************************************************************
//Function:
bool IGUI::listBox(const std::string& vLabel, int &voCurrentItem, const std::vector<std::string>& vItems, int vDisplayedNumInBox/* = -1*/)
{
	std::vector<const char*> Items;
	for (const auto &e : vItems)
		Items.push_back(e.c_str());
	return ImGui::ListBox(vLabel.c_str(), &voCurrentItem, Items.data(), static_cast<int>(vItems.size()), vDisplayedNumInBox);
}

//************************************************************************************
//Function:
bool IGUI::beginChild(const std::string& vLabel, const glm::vec2& vSize /* = glm::vec2(0) */, bool vIsBorder /* = true */)
{
	return ImGui::BeginChild(vLabel.c_str(), ImVec2(vSize[0], vSize[1]), vIsBorder);
}

//************************************************************************************
//Function:
void IGUI::endChild()
{
	ImGui::EndChild();
}

//************************************************************************************
//Function:
void IGUI::columns(int vColNum, const std::string& vNameId /* = "" */, bool vIsBorder /* = true */)
{
	ImGui::Columns(vColNum, vNameId.c_str(), vIsBorder);
}

//************************************************************************************
//Function:
void IGUI::nextColumn()
{
	ImGui::NextColumn();
}

//************************************************************************************
//Function:
bool IGUI::begin(const std::string& vName)
{
	return ImGui::Begin(vName.c_str());
}

//************************************************************************************
//Function:
bool IGUI::begin(const std::string &vName, bool &voIsOpen)
{
	return ImGui::Begin(vName.c_str(), &voIsOpen);
}

//************************************************************************************
//Function:
void IGUI::end()
{
	ImGui::End();
}

//************************************************************************************
//Function:
bool IGUI::beginPopupContextItem()
{
	return ImGui::BeginPopupContextItem();
}

//************************************************************************************
//Function:
void IGUI::endPopup()
{
	ImGui::EndPopup();
}

//************************************************************************************
//Function:
void IGUI::openPopup(const std::string& vNameId)
{
	ImGui::OpenPopup(vNameId.c_str());
}

//************************************************************************************
//Function:
bool IGUI::beginPopup(const std::string& vNameId)
{
	return ImGui::BeginPopup(vNameId.c_str());
}

//************************************************************************************
//Function:
bool IGUI::beginPopupModal(const std::string& vNameId)
{
	return ImGui::BeginPopupModal(vNameId.c_str());
}

//************************************************************************************
//Function:
void IGUI::closeCurrentPopup()
{
	ImGui::CloseCurrentPopup();
}

//************************************************************************************
//Function:
void IGUI::pushStyleColor(ElayGraphics::EGUIItemColor vItemColor, const glm::vec4& vColor)
{
	ImGui::PushStyleColor(static_cast<int>(vItemColor), (ImVec4)ImColor::HSV(vColor[0], vColor[1], vColor[2], vColor[3]));
}

//************************************************************************************
//Function:
void IGUI::popStyleColor(int vPopCount/* = 1*/)
{
	ImGui::PopStyleColor(vPopCount);
}