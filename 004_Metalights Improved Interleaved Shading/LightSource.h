#pragma once
#include "GameObject.h"
#include <GLM/glm.hpp>
#include <vector>
using namespace std;
struct SPointLight
{
	glm::vec4  Position;
	glm::vec4  ColorAndRadius;
	glm::vec4 Importance;
};

class CLightSources : public IGameObject
{
public:
	CLightSources(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~CLightSources();
	CLightSources();
	static CLightSources& get_instance() {
		static CLightSources instance;
		return instance;
	};
	virtual void initV() override;
	virtual void updateV() override;
	vector<SPointLight> *getPointLights();
private:
	std::vector<SPointLight> *m_LightSources;
	int m_LightNum = 10000;
};