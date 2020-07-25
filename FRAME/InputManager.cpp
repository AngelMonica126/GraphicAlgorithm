#include "InputManager.h"
#include <crtdbg.h>
#include "GLFWWindow.h"
#include "ResourceManager.h"

GLint CInputManager::m_CursorStatus = 1;
std::array<GLboolean, 1024>  CInputManager::m_KeysStatus         = { GL_FALSE };
std::array<GLboolean, 3>     CInputManager::m_MouseButtonStatus  = { GL_FALSE };
std::array<GLdouble, 2>      CInputManager::m_CursorPos          = { 0.0 };
std::array<GLdouble, 2>      CInputManager::m_CursorOffset       = { 0.0 };
std::array<GLdouble, 2>      CInputManager::m_CursorPosLastFrame = { 0.0 };
std::array<GLdouble, 2>      CInputManager::m_ScrollJourney      = { 0.0 };
std::vector<std::function<GLvoid(GLint, GLint, GLint)>>          CInputManager::m_MouseButtonCallbackResponseFuncSet;
std::vector<std::function<GLvoid(GLdouble, GLdouble)>>           CInputManager::m_CursorCallbackResponseFuncSet;
std::vector<std::function<GLvoid(GLdouble, GLdouble)>>           CInputManager::m_ScrollCallbackResponseFuncSet;
std::vector<std::function<GLvoid(GLint, GLint, GLint, GLint)>>   CInputManager::m_KeyCallbackResponseFuncSet;

CInputManager::CInputManager()
{
}

CInputManager::~CInputManager()
{
}

//************************************************************************************
//Function:
GLvoid CInputManager::init()
{
	__registerCallbackFunc();
}

//************************************************************************************
//Function:
GLvoid CInputManager::__registerCallbackFunc()
{
	GLFWwindow* pWindow = CResourceManager::getOrCreateInstance()->fetchOrCreateGLFWWindow()->fetchWindow();
	glfwSetKeyCallback(pWindow, __keyCallbackFunc);
	glfwSetMouseButtonCallback(pWindow, __mouseButtonCallbackFunc);
	glfwSetCursorPosCallback(pWindow, __cursorCallbackFunc);
	glfwSetScrollCallback(pWindow, __scrollCallbackFunc);
}

//************************************************************************************
//Function:
GLvoid CInputManager::__keyCallbackFunc(GLFWwindow *vWindow, GLint vKey, GLint vScancode, GLint vAction, GLint vMode)
{
	if (vKey == GLFW_KEY_ESCAPE && vAction == GLFW_PRESS)
		glfwSetWindowShouldClose(vWindow, GL_TRUE);

	m_KeysStatus[vKey] = vAction;

	if (m_KeysStatus[GLFW_KEY_LEFT_ALT] == GLFW_PRESS && m_KeysStatus[GLFW_KEY_C] == GLFW_PRESS)
	{
		m_CursorStatus = !m_CursorStatus;
		glfwSetInputMode(CResourceManager::getOrCreateInstance()->fetchOrCreateGLFWWindow()->fetchWindow(), GLFW_CURSOR, m_CursorStatus ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		if (m_CursorStatus == 1)
			m_CursorStatus = 2;
	}
	
	if (!m_KeyCallbackResponseFuncSet.empty())
	{
		for (auto vIt : m_KeyCallbackResponseFuncSet)
			vIt(vKey, vScancode, vAction, vMode);
	}
}

//************************************************************************************
//Function:
GLvoid CInputManager::__mouseButtonCallbackFunc(GLFWwindow* vWindow, GLint vButton, GLint vAction, GLint vMods)
{
	m_MouseButtonStatus[vButton - GLFW_MOUSE_BUTTON_LEFT] = vAction;   //GLFWÖÐdefine GLFW_MOUSE_BUTTON_LEFT 0£¬define GLFW_PRESS 1
	
	if (!m_MouseButtonCallbackResponseFuncSet.empty())
	{
		for (auto vIt : m_MouseButtonCallbackResponseFuncSet)
			vIt(vButton, vAction, vMods);
	}
}

//************************************************************************************
//Function:
GLvoid CInputManager::__cursorCallbackFunc(GLFWwindow* vWindow, GLdouble vPosX, GLdouble vPosY)
{
	m_CursorPos[0] = vPosX;
	m_CursorPos[1] = vPosY;
	if (m_CursorStatus == 2)
	{
		m_CursorOffset[1] = m_CursorOffset[0] = 0;
		m_CursorStatus = 1;
	}
	else
	{
		m_CursorOffset[0] = m_CursorPos[0] - m_CursorPosLastFrame[0];
		m_CursorOffset[1] = m_CursorPosLastFrame[1] - m_CursorPos[1];
	}
	m_CursorPosLastFrame = m_CursorPos;

	if (!m_CursorCallbackResponseFuncSet.empty())
	{
		for (auto vIt : m_CursorCallbackResponseFuncSet)
			vIt(vPosX, vPosY);
	}
}

//************************************************************************************
//Function:
GLvoid CInputManager::__scrollCallbackFunc(GLFWwindow* vWindow, GLdouble vOffsetX, GLdouble vOffsetY)
{
	m_ScrollJourney[0] += vOffsetX;
	m_ScrollJourney[1] += vOffsetY;

	if (!m_ScrollCallbackResponseFuncSet.empty())
	{
		for (auto vIt : m_ScrollCallbackResponseFuncSet)
			vIt(vOffsetX, vOffsetY);
	}
}

//************************************************************************************
//Function:
const std::array<GLboolean, 1024>& CInputManager::getKeyStatus() const
{
	return m_KeysStatus;
}

//************************************************************************************
//Function:
const std::array<GLboolean, 3>& CInputManager::getMouseButtonStatus() const
{
	return m_MouseButtonStatus;
}

//************************************************************************************
//Function:
const std::array<GLdouble, 2>& CInputManager::getCursorPos() const
{
	return m_CursorPos;
}

const std::array<GLdouble, 2>& CInputManager::getCursorOffset() const
{
	return m_CursorOffset;
}

//************************************************************************************
//Function:
const std::array<GLdouble, 2>& CInputManager::getScrollJourney() const
{
	return m_ScrollJourney;
}

//************************************************************************************
//Function:
GLint CInputManager::registerCursorCallbackFunc(std::function<GLvoid(GLdouble, GLdouble)> vCursorCallbackFunc)
{
	m_CursorCallbackResponseFuncSet.push_back(vCursorCallbackFunc);
	return static_cast<int>(m_CursorCallbackResponseFuncSet.size()) - 1;
}

//************************************************************************************
//Function:
GLint CInputManager::registerKeyCallbackFunc(std::function<GLvoid(GLint, GLint, GLint, GLint)> vKeyCallbackFunc)
{
	m_KeyCallbackResponseFuncSet.push_back(vKeyCallbackFunc);
	return static_cast<int>(m_KeyCallbackResponseFuncSet.size()) - 1;
}

//************************************************************************************
//Function:
GLint CInputManager::registerMouseButtonCallbackFunc(std::function<GLvoid(GLint, GLint, GLint)> vMouseButtonCallbackFunc)
{
	m_MouseButtonCallbackResponseFuncSet.push_back(vMouseButtonCallbackFunc);
	return static_cast<int>(m_MouseButtonCallbackResponseFuncSet.size()) - 1;
}

//************************************************************************************
//Function:
GLint CInputManager::registerScrollCallbackFunc(std::function<GLvoid(GLdouble, GLdouble)> vScrollCallbackFunc)
{
	m_ScrollCallbackResponseFuncSet.push_back(vScrollCallbackFunc);
	return static_cast<int>(m_ScrollCallbackResponseFuncSet.size()) - 1;
}

//************************************************************************************
//Function:  no test...
GLvoid CInputManager::unregisterKeyCallbackFunc(GLint vEraseIndex)
{
	m_ScrollCallbackResponseFuncSet.erase(m_ScrollCallbackResponseFuncSet.begin() + vEraseIndex);
}

//************************************************************************************
//Function:
GLvoid CInputManager::unregisterCursorCallbackFunc(GLint vEraseIndex)
{
	m_CursorCallbackResponseFuncSet.erase(m_CursorCallbackResponseFuncSet.begin() + vEraseIndex);
}

//************************************************************************************
//Function:
GLvoid CInputManager::unregisterScrollCallbackFunc(GLint vEraseIndex)
{
	m_ScrollCallbackResponseFuncSet.erase(m_ScrollCallbackResponseFuncSet.begin() + vEraseIndex);
}

//************************************************************************************
//Function:
GLvoid CInputManager::unregisterMouseButtonCallbackFunc(GLint vEraseIndex)
{
	m_MouseButtonCallbackResponseFuncSet.erase(m_MouseButtonCallbackResponseFuncSet.begin() + vEraseIndex);
}

//************************************************************************************
//Function:
GLvoid CInputManager::unregisterKeyCallbackFunc(std::function<GLvoid(GLint, GLint, GLint, GLint)> vKeyCallbackFunc)
{
	//To do...
}

//************************************************************************************
//Function:
GLvoid CInputManager::unregisterCursorCallbackFunc(std::function<GLvoid(GLdouble, GLdouble)> vCursorCallbackFunc)
{
	//To do...
}

//************************************************************************************
//Function:
GLvoid CInputManager::unregisterScrollCallbackFunc(std::function<GLvoid(GLdouble, GLdouble)> vScrollCallbackFunc)
{
	//To do...
}

//************************************************************************************
//Function:
GLvoid CInputManager::unregisterMouseButtonCallbackFunc(std::function<GLvoid(GLint, GLint, GLint)> vMouseButtonCallbackFunc)
{
	//To do...
}