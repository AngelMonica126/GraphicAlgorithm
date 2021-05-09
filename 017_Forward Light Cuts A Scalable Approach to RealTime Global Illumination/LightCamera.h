#pragma once
#include "GameObject.h"
#include <GLM/glm.hpp>

class CLightCamera : public IGameObject
{
public:
	CLightCamera(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~CLightCamera();

	virtual void initV() override;
	virtual void updateV() override;

private:
	glm::vec3 m_LightDir;
	glm::vec3 m_LightPos;
	glm::vec3 m_LightUpVector = glm::vec3(0, 1, 0);
	glm::mat4 m_LightViewMatrix;
	glm::mat4 m_LightProjectionMatrix;
	float m_Exposure = 2.0f;
	float m_CameraSizeExtent = 1;// 0.25f;
};