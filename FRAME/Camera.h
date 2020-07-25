#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

class CCamera
{
public:
	CCamera(glm::dvec3 vCameraPos = glm::dvec3(0.0, 0.0, 3.0), glm::dvec3 vCameraFront = glm::dvec3(0.0, 0.0, -1.0), glm::dvec3 vUpVector = glm::dvec3(0.0, 1.0, 0.0));
	~CCamera();
	
	GLvoid init();
	GLvoid update();

	GLdouble   getCameraFov() const;
	GLdouble   getCameraNear() const;
	GLdouble   getCameraFar() const;
	glm::mat4  getViewMatrix() const;  //not need const reference
	glm::mat4  getPrevViewMatrix() const;
	glm::mat4  getProjectionMatrix() const;
	glm::dvec3 getLookAtPos() const;
	glm::dvec3 getCameraFront() const;
	const glm::dvec3& getCameraPos() const;
	const glm::dvec3& getCameraUp() const;

	void setCameraPos(glm::dvec3 vCameraPos) { m_CameraPos = vCameraPos; }
	void setCameraFront(glm::dvec3 vCameraFront) { m_CameraFront = vCameraFront; }
	void setFarPlane(GLdouble vFarPlane) { m_Far = vFarPlane; }
	void setMoveSpeed(GLdouble vMoveSpeed) { m_MoveSpeed = vMoveSpeed; }
	void setFov(GLdouble vFov) { m_Fov = vFov; }
	void setEnableCursor(bool vIsEnableCursor) { m_IsEnableCursor = vIsEnableCursor; }

private:
	GLvoid __processMovement4KeyCallback(GLint vKey, GLint vScancode, GLint vAction, GLint vMode);
	GLvoid __processRotate4CursorCallback(GLdouble vPosX, GLdouble vPosY);
	GLvoid __processScroll4ScrollCallback(GLdouble vOffsetX, GLdouble vOffsetY);
	GLvoid __processMouseButton4MouseButtonCallback(GLint vButton, GLint vAction, GLint vMods);

	glm::dvec3 m_CameraPos;
	glm::dvec3 m_PrevCameraPos;
	glm::dvec3 m_UpVector;		//up vector never change, so no need prevUpVector
	glm::dvec3 m_CameraFront;
	glm::dvec3 m_PrevCameraFront;
	glm::dvec3 m_CameraRight = glm::vec3(0.0);
	glm::mat4 m_ViewMatrix = glm::mat4();
	glm::mat4 m_ProjectionMatrix = glm::mat4();
	GLdouble m_Pitch = 0.0;
	GLdouble m_Yaw = 0.0;
	GLdouble m_Fov = 45.0;
	GLdouble m_MoveSpeed = 5.0;
	GLdouble m_Sensitivity =0.03;
	GLdouble m_Near = 0.1;
	GLdouble m_Far = 100.0;
	GLboolean m_IsEnableCursor = true;
};