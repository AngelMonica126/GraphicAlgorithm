//--------------------------------------------------------------------------------------
// Copyright (c) Elay Pu. All rights reserved.
//--------------------------------------------------------------------------------------

#include "Camera.h"
#include <iostream>
#include "InputManager.h"
#include "App.h"
#include "common.h"

CCamera::CCamera(glm::dvec3 vCameraPos, glm::dvec3 vCameraFront, glm::dvec3 vUpVector) :m_CameraPos(vCameraPos), m_CameraFront(vCameraFront), m_UpVector(vUpVector), m_PrevCameraPos(vCameraPos), m_PrevCameraFront(vCameraFront)
{
	m_CameraRight = glm::normalize(glm::cross(m_UpVector, -m_CameraFront));  //Fixed Me: - ?
	m_Pitch = asin(m_CameraFront.y);
	m_Yaw = asin(m_CameraFront.z / cos(m_Pitch));
}

CCamera::~CCamera()
{
}

//************************************************************************************
//Function:
GLvoid CCamera::init()
{
	std::shared_ptr<CInputManager> &pInputManager = CInputManager::getOrCreateInstance();
	pInputManager->registerKeyCallbackFunc(std::bind(&CCamera::__processMovement4KeyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	pInputManager->registerCursorCallbackFunc(std::bind(&CCamera::__processRotate4CursorCallback, this, std::placeholders::_1, std::placeholders::_2));
	pInputManager->registerScrollCallbackFunc(std::bind(&CCamera::__processScroll4ScrollCallback, this, std::placeholders::_1, std::placeholders::_2));
	pInputManager->registerMouseButtonCallbackFunc(std::bind(&CCamera::__processMouseButton4MouseButtonCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

//************************************************************************************
//Function:
GLvoid CCamera::__processMovement4KeyCallback(GLint vKey, GLint vScancode, GLint vAction, GLint vMode)
{
}

//************************************************************************************
//Function:
GLvoid CCamera::__processRotate4CursorCallback(GLdouble vPosX, GLdouble vPosY)
{
	if (m_IsEnableCursor)
	{
		std::array<double, 2> CursorOffset = CInputManager::getOrCreateInstance()->getCursorOffset();
		CursorOffset[0] *= m_Sensitivity;
		CursorOffset[1] *= m_Sensitivity;
		m_Yaw += glm::radians(CursorOffset[0]);
		m_Pitch += glm::radians(CursorOffset[1]);
		if (m_Pitch > glm::radians(89.0))
			m_Pitch = glm::radians(89.0);
		else if (m_Pitch < glm::radians(-89.0))
			m_Pitch = glm::radians(-89.0);
		m_CameraFront.x = cos(m_Pitch) * cos(m_Yaw);
		m_CameraFront.y = sin(m_Pitch);
		m_CameraFront.z = cos(m_Pitch) * sin(m_Yaw);
		m_CameraFront = glm::normalize(m_CameraFront);
		m_CameraRight = normalize(cross(m_UpVector, -m_CameraFront));
	}
}

//************************************************************************************
//Function:
GLvoid CCamera::__processScroll4ScrollCallback(GLdouble vOffsetX, GLdouble vOffsetY)
{
	if (m_Fov >= 1.0 && m_Fov <= 45.0)
		m_Fov -= vOffsetY;
	if (m_Fov < 1.0)
		m_Fov = 1.0;
	else if (m_Fov > 45.0)
		m_Fov = 45.0;
}

//************************************************************************************
//Function:
GLvoid CCamera::__processMouseButton4MouseButtonCallback(GLint vButton, GLint vAction, GLint vMods)
{
	if (vButton == GLFW_MOUSE_BUTTON_RIGHT && vAction == GLFW_PRESS)
	{
		std::cout << "CameraPos: " << m_CameraPos.x << ", " << m_CameraPos.y << ", " << m_CameraPos.z << std::endl;
		std::cout << "CameraFront: " << m_CameraFront.x << ", " << m_CameraFront.y << ", " << m_CameraFront.z << std::endl;
	}
}

//************************************************************************************
//Function:
GLdouble CCamera::getCameraFov() const 
{
	return m_Fov;
}

//************************************************************************************
//Function:
GLdouble CCamera::getCameraNear() const
{
	return m_Near;
}

//************************************************************************************
//Function:
GLdouble CCamera::getCameraFar() const
{
	return m_Far;
}

//************************************************************************************
//Function:
const glm::dvec3& CCamera::getCameraPos() const
{
	return m_CameraPos;
}

//************************************************************************************
//Function:
glm::dvec3 CCamera::getLookAtPos() const
{
	return m_CameraPos + m_CameraFront;
}

//************************************************************************************
//Function:
glm::dvec3 CCamera::getCameraFront() const
{
	return m_CameraFront;
}

//************************************************************************************
//Function:
const glm::dvec3& CCamera::getCameraUp() const
{
	return m_UpVector;
}

//************************************************************************************
//Function:
glm::mat4 CCamera::getViewMatrix() const 
{
	return glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_UpVector);
}

//************************************************************************************
//Function:
glm::mat4 CCamera::getProjectionMatrix() const
{
	//return glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 1000.0f);;
	return glm::perspective(glm::radians(m_Fov), (GLdouble)ElayGraphics::WINDOW_KEYWORD::WINDOW_WIDTH / (GLdouble)ElayGraphics::WINDOW_KEYWORD::WINDOW_HEIGHT, m_Near, m_Far);
}

//************************************************************************************
//Function:
void CCamera::update()
{
	m_PrevCameraPos = m_CameraPos;
	m_PrevCameraFront = m_CameraFront;

	std::shared_ptr<CInputManager> pInputManager = CInputManager::getOrCreateInstance();
	GLdouble MoveDistance = CApp::getOrCreateInstance()->getDeltaTime() * m_MoveSpeed;
	if (pInputManager->getKeyStatus()[GLFW_KEY_W])
		m_CameraPos += MoveDistance * glm::dvec3(m_CameraFront.x, 0, m_CameraFront.z);
	if (pInputManager->getKeyStatus()[GLFW_KEY_S])
		m_CameraPos -= MoveDistance * glm::dvec3(m_CameraFront.x, 0, m_CameraFront.z);
	if (pInputManager->getKeyStatus()[GLFW_KEY_D])
		m_CameraPos += MoveDistance * m_CameraRight;
	if (pInputManager->getKeyStatus()[GLFW_KEY_A])
		m_CameraPos -= MoveDistance * m_CameraRight;
	if (pInputManager->getKeyStatus()[GLFW_KEY_Q])
		m_CameraPos -= MoveDistance * glm::dvec3(0.0, 1.0, 0.0);
	if (pInputManager->getKeyStatus()[GLFW_KEY_E])
		m_CameraPos += MoveDistance * glm::dvec3(0.0, 1.0, 0.0);
	if (pInputManager->getKeyStatus()[GLFW_KEY_Z])
		m_MoveSpeed *= 1.2f;
	if (pInputManager->getKeyStatus()[GLFW_KEY_C])
		m_MoveSpeed /= 1.2f;
}

//************************************************************************************
//Function:
glm::mat4 CCamera::getPrevViewMatrix() const
{
	return glm::lookAt(m_PrevCameraPos, m_PrevCameraPos + m_PrevCameraFront, m_UpVector);
}