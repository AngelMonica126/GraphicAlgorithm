#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

class CRCASPass : public IRenderPass
{
public:
	CRCASPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CRCASPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	glm::mat4 m_LightVPMatrix;
	glm::vec3 m_LightDir;
};