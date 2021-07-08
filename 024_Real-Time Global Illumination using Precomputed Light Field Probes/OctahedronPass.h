#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

class COctahedronPass : public IRenderPass
{
public:
	COctahedronPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~COctahedronPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	glm::vec3 m_LightDir;
};