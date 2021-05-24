#pragma once
#include "GameObject.h"
#include <GLM/glm.hpp>
#include "vector"
#define  SplitNum 8
struct FrustumPos
{
	float Near;
	float Far;
	std::vector<glm::vec3> Pos;
};
class CLightCamera : public IGameObject
{
public:
	CLightCamera(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~CLightCamera();

	virtual void initV() override;
	virtual void updateV() override;
	void initFrustum();
	void setFrustum();
	std::vector<glm::mat4> &getFrustum() { return m_Frustum; }

private:
	glm::vec3 m_LightDir;
	glm::vec3 m_LightPos;
	glm::vec3 m_LightUpVector = glm::vec3(0, 1, 0);
	glm::mat4 m_LightViewMatrix;
	float m_Exposure = 2.0f;
	
	int m_SplitNum = 8;
	float m_Weight = 0.75;
	std::vector<FrustumPos> m_FrustumNF;
	std::vector<glm::mat4> m_Frustum;
};