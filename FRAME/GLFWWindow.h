#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

class CGLFWWindow
{
public:
	CGLFWWindow();
	~CGLFWWindow();

	void init();
	GLFWwindow* fetchWindow() const;


private:
	void __setViewport();

	GLFWwindow *m_pWindow = nullptr;				//If set m_pWindow as shared_ptr, will result in warning: delete incomplete type pointer, no call destructor
};