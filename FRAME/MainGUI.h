#pragma once
#include "FRAME_EXPORTS.h"
#include <memory>
#include <map>
#include <vector>

struct ImGuiIO;
struct ImFont;

class CMainGUI
{
public:
	void init();
	void update();
	void lateUpdate();

	ImGuiIO* getIO() const { return m_pIO; }

private:
	ImGuiIO*					m_pIO = nullptr;							//use share_ptr will cause error when program exits
	std::shared_ptr<ImFont>		m_pDefaultFont;
	std::vector<unsigned short>	m_IconRanges = { 0xf000,0xf3ff,0 };
	bool m_IsShowDefaultWindow = true;
	bool m_IsEndDefaultWindowInCurrentFrame = false;
};