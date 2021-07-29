//--------------------------------------------------------------------------------------
// Copyright (c) Elay Pu. All rights reserved.
//--------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
//The input manager designs two working patterns:
//1. Other classes can get the input state through a series of "get..." member functions.
//2. Other classes can register the response function corresponding input through a series of register 
//   member functions, and the GLFW will execute the functions that have been registered when the callback 
//   function is executed.
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <vector>
#include <array>
#include "Singleton.h"
#include "FRAME_EXPORTS.h"

class CInputManager : public CSingleton<CInputManager>
{
	friend class CSingleton<CInputManager>;
public:
	~CInputManager();

	GLvoid init();

	GLint registerKeyCallbackFunc(std::function<GLvoid(GLint, GLint, GLint, GLint)> vKeyCallbackFunc);
	GLint registerCursorCallbackFunc(std::function<GLvoid(GLdouble, GLdouble)> vCursorCallbackFunc);
	GLint registerScrollCallbackFunc(std::function<GLvoid(GLdouble, GLdouble)> vScrollCallbackFunc);
	GLint registerMouseButtonCallbackFunc(std::function<GLvoid(GLint, GLint, GLint)> vMouseButtonCallbackFunc);

	GLvoid unregisterKeyCallbackFunc(std::function<GLvoid(GLint, GLint, GLint, GLint)> vKeyCallbackFunc);
	GLvoid unregisterCursorCallbackFunc(std::function<GLvoid(GLdouble, GLdouble)> vCursorCallbackFunc);
	GLvoid unregisterScrollCallbackFunc(std::function<GLvoid(GLdouble, GLdouble)> vScrollCallbackFunc);
	GLvoid unregisterMouseButtonCallbackFunc(std::function<GLvoid(GLint, GLint, GLint)> vMouseButtonCallbackFunc);

	GLvoid unregisterKeyCallbackFunc(GLint vEraseIndex);
	GLvoid unregisterCursorCallbackFunc(GLint vEraseIndex);
	GLvoid unregisterScrollCallbackFunc(GLint vEraseIndex);
	GLvoid unregisterMouseButtonCallbackFunc(GLint vEraseIndex);

	const std::array<GLboolean, 1024>& getKeyStatus() const;
	const std::array<GLboolean, 3>&    getMouseButtonStatus() const;
	const std::array<GLdouble, 2>&     getCursorPos() const;
	const std::array<GLdouble, 2>&     getCursorOffset() const;
	const std::array<GLdouble, 2>&     getScrollJourney() const;
	GLboolean						   getCursorStatus() const { return m_CursorStatus; }

	void setCursorStatus(GLboolean vCursorStatus) { m_CursorStatus; }

private:
	static GLint m_CursorStatus;

	static std::array<GLboolean,1024>     m_KeysStatus;
	static std::array<GLboolean,3>        m_MouseButtonStatus;
	static std::array<GLdouble,2>         m_CursorPos;
	static std::array<GLdouble,2>         m_CursorPosLastFrame;
	static std::array<GLdouble,2>         m_CursorOffset;
	static std::array<GLdouble,2>         m_ScrollJourney;		//滚轮从一开始累计滑动的路程

	static std::vector<std::function<GLvoid(GLint, GLint, GLint)>>        m_MouseButtonCallbackResponseFuncSet;
	static std::vector<std::function<GLvoid(GLdouble, GLdouble)>>         m_CursorCallbackResponseFuncSet;
	static std::vector<std::function<GLvoid(GLdouble, GLdouble)>>         m_ScrollCallbackResponseFuncSet;
	static std::vector<std::function<GLvoid(GLint, GLint, GLint, GLint)>> m_KeyCallbackResponseFuncSet;

	CInputManager();
	GLvoid __registerCallbackFunc();
	static GLvoid __keyCallbackFunc(GLFWwindow *vWindow, GLint vKey, GLint vScancode, GLint vAction, GLint vMode);
	static GLvoid __cursorCallbackFunc(GLFWwindow* vWindow, GLdouble vPosX, GLdouble vPosY);
	static GLvoid __scrollCallbackFunc(GLFWwindow* vWindow, GLdouble vOffsetX, GLdouble vOffsetY);
	static GLvoid __mouseButtonCallbackFunc(GLFWwindow* vWindow, GLint vButton, GLint vAction, GLint vMods);
};