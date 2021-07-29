//--------------------------------------------------------------------------------------
// Copyright (c) Elay Pu. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include "FRAME_EXPORTS.h"
#include <memory>
#include <map>
#include <vector>
#include <boost/any.hpp>
#include "Common.h"
#include <GLM/glm.hpp>

struct ImFont;

class FRAME_DLLEXPORTS IGUI
{
public:
	IGUI();
	IGUI(const std::string& vName, int vExcutionOrder);
	virtual ~IGUI();

	virtual void initV() {}
	virtual void updateV() {}

	bool operator<(const IGUI& vOtherGUI) const;

	void setName(const std::string& vName) { m_Name = vName; }
	void setExecutionOrder(int vExecutionOrder) { m_ExecutionOrder = vExecutionOrder; }

	int getExecutionOrder() const { return m_ExecutionOrder; }
	const std::string& getName()const { return m_Name; }
	const std::shared_ptr<ImFont>& getFontByName(const std::string& vFontName);

	void loadFont(const std::string& vFontFilePath, float vFontSize);
	bool combo(const std::string& vLabel, int &vioCurrentItem, const std::vector<std::string>& vItems);
	void pushFont(const std::shared_ptr<ImFont>& vFont);	//need to be paired with popFont
	void popFont();
	void text(const std::string& vText);
	void textDisabled(const std::string& vText);
	void textColored(const glm::vec4& vColor, const std::string& vText);
	void bulletText(const std::string& vText);
	bool beginMenuBar();						//need to be paired with endMenuBar
	void endMenuBar();
	bool beginMenu(const std::string& vLable);	//need to be paired with endMenu
	void endMenu();
	bool menuItem(const std::string& vLabel);
	bool checkBox(const std::string& vLable, bool &voCheckValue);	//return true once clicked, while voValue = true when checkmark appears.
	void verticalSpacing();
	bool sliderScalar(const std::string& vLable, ElayGraphics::EDataType vDataType, void* vValue, const void* vMin, const void* vMax);			//return true when value changed
	bool sliderScalarN(const std::string& vLable, ElayGraphics::EDataType vDataType, int vComponentsNum, void* vValue, const void* vMin, const void* vMax);
	bool sliderFloat4(const std::string& vLable, glm::vec4& vioValue, float vMin, float vMax);
	bool verticalSilderScalar(const std::string& vLable, const glm::vec2& vSliderSize, ElayGraphics::EDataType vDataType, void* vValue, const void* vMin, const void* vMax);	//return true when value changed
	void horizontalLine();
	void sameLine();
	bool button(const std::string& vLable, const glm::vec2& vSize = glm::vec2(0));
	bool colorButton(const std::string& vLable, const glm::vec4& vColor, const glm::vec2& vSize = glm::vec2(0));
	void setColorPickerType(int vPickerType);	//0: Bar, !0: Wheel
	bool colorEdit(const std::string& vLable, glm::vec4& voColor, bool vIsEditAlpha = true);	//return true when color changed
	bool collapsingHeader(const std::string& vLabel);
	bool treeNode(const std::string& vLable);	//need to be paired with treePop
	void treePop();
	void bullet();
	void indent();
	void unIndent();
	bool isLastItemHovered();
	void beginTooltip();						//need to be paired with endTooltip
	void endTooltip();
	void promptMarker(const std::string& vPromptMarker, const std::string& vPromptText);
	bool radioButton(const std::string& vLabel, int &voValue, int vRadioButtonIndex);
	void plotLines(const std::string& vLabel, const std::vector<float> &vValueSet, float vMin = MAX_VALUE, float vMax = MAX_VALUE, const glm::vec2& vSize = glm::vec2(0));
	void plotLines(const std::string& vLabel, float(*Func)(void*, int), int vSampleCount, float vMin = MAX_VALUE, float vMax = MAX_VALUE, const glm::vec2& vSize = glm::vec2(0));
	void plotHistogram(const std::string& vLable, const std::vector<float> &vValueSet, float vMin = MAX_VALUE, float vMax = MAX_VALUE, const glm::vec2& vSize = glm::vec2(0));
	void plotHistogram(const std::string& vLabel, float(*Func)(void*, int), int vSampleCount, float vMin = MAX_VALUE, float vMax = MAX_VALUE, const glm::vec2& vSize = glm::vec2(0));
	void processBar(float vProcessValue, const glm::vec2& vSize = glm::vec2(0, 1));
	bool inputText(const std::string& vLabel, std::string& voValue, size_t vValueBufferSize = 128, bool vIsPassword = false);
	bool inputInt(const std::string& vLabel, int &voValue);
	bool inputFloat(const std::string& vLabel, float &voValue);
	bool inputDouble(const std::string& vLabel, double &voValue);
	bool inputFloat3(const std::string& vLabel, glm::vec3& voValue);
	bool inputFloat4(const std::string& vLabel, glm::vec4& voValue);
	void pushItemWidth(float vItemWidth);
	void popItemWidth();
	bool dragScalar(const std::string& vLabel, ElayGraphics::EDataType vDataType, void* vValue, float vDragSpeed, const void* vMin, const void* vMax);
	bool dragScalarN(const std::string& vLabel, ElayGraphics::EDataType vDataType, void* vValue, int vComponentsNum, float vDragSpeed, const void* vMin, const void* vMax);
	bool listBox(const std::string& vLabel, int &voCurrentItem, const std::vector<std::string>& vItems, int vDisplayedNumInBox = -1);
	bool beginChild(const std::string& vLabel, const glm::vec2& vSize = glm::vec2(0), bool vIsBorder = true);	//need to be paired with endChild
	void endChild();
	void columns(int vColNum, const std::string& vNameId="", bool vIsBorder = true);
	void nextColumn();
	bool begin(const std::string& vName);
	bool begin(const std::string &vName, bool &voIsOpen);
	void end();
	bool beginPopupContextItem();	//On the last Item, the right-click pops up, which is equivalent to "if (IsItemHovered () & & IsMouseReleased (0)) OpenPopup (id); return BeginPopup (id);"
	void endPopup();
	void openPopup(const std::string& vNameId);
	bool beginPopup(const std::string& vNameId);	//return true when openPopup has be called
	bool beginPopupModal(const std::string& vNameId);	//A dialog window pops up, the background window loses focus, and cannot be closed by clicking on an external area
	void closeCurrentPopup();
	void pushStyleColor(ElayGraphics::EGUIItemColor vItemColor, const glm::vec4& vColor);
	void popStyleColor(int vPopCount = 1);

private:
	int					 m_ExecutionOrder = -1;
	std::string			 m_Name;
	std::vector<unsigned short> m_IconRanges = { 0xf000,0xf3ff,0 };
	std::map<std::string, std::shared_ptr<ImFont>> m_FontMap;
};