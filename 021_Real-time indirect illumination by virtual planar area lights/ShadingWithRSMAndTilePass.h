#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

class CShadingWithRSMAndTilePass : public IRenderPass
{
public:
	CShadingWithRSMAndTilePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CShadingWithRSMAndTilePass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	glm::vec3 m_LightDir;
};