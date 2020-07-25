#pragma once
#include "GameObject.h"
#include <GLM/glm.hpp>
#include <vector>

struct SPointLight
{
	glm::vec4  Position;
	glm::vec4  ColorAndRadius;
};

class CLightSources : public IGameObject
{
public:
	CLightSources(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~CLightSources();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<SPointLight> m_LightSources;
	int m_LightNum = 10000;
};