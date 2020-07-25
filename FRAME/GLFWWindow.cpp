#include "GLFWWindow.h"
#include <iostream>
#include "common.h"
//#include "Utils.h"

CGLFWWindow::CGLFWWindow() : m_pWindow(nullptr)
{
}

CGLFWWindow::~CGLFWWindow()
{
}

//************************************************************************************
//Function:
void CGLFWWindow::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//#ifdef MULTISAMPLE
	glfwWindowHint(GLFW_SAMPLES, ElayGraphics::WINDOW_KEYWORD::NUM_SAMPLES);
//#endif //MULTISAMPLE
	//m_pWindow = std::make_shared<GLFWwindow>(glfwCreateWindow(ElayGraphics::WINDOW_KEYWORD::WINDOW_WIDTH, ElayGraphics::WINDOW_KEYWORD::WINDOW_HEIGHT, ElayGraphics::WINDOW_KEYWORD::WINDOW_TITLE.c_str(), nullptr, nullptr));
	m_pWindow = glfwCreateWindow(ElayGraphics::WINDOW_KEYWORD::WINDOW_WIDTH, ElayGraphics::WINDOW_KEYWORD::WINDOW_HEIGHT, ElayGraphics::WINDOW_KEYWORD::WINDOW_TITLE.c_str(), nullptr, nullptr);
	if (!m_pWindow)
	{
		std::cerr << "Error::Window:: Window Create Failure" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_pWindow);
	if(ElayGraphics::WINDOW_KEYWORD::CURSOR_DISABLE)
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Error::Window:: GLEW Init Failure" << std::endl;
		return;
	}
	__setViewport();
}

//************************************************************************************
//Function:
void CGLFWWindow::__setViewport()
{
	glViewport(ElayGraphics::WINDOW_KEYWORD::VIEWPORT_LEFTBOTTOM_X, ElayGraphics::WINDOW_KEYWORD::VIEWPORT_LEFTBOTTOM_Y, ElayGraphics::WINDOW_KEYWORD::VIEWPORT_WIDTH, ElayGraphics::WINDOW_KEYWORD::VIEWPORT_HEIGHT);
}

//************************************************************************************
//Function:
GLFWwindow* CGLFWWindow::fetchWindow() const
{
	return m_pWindow;
}