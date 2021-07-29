#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

class CLightCamera;
class CCSMDebugPass : public IRenderPass
{
public:
	CCSMDebugPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CCSMDebugPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<CLightCamera> m_pCLightCamera;
	std::vector<int> m_GlobalGroupSize;
	glm::mat4 m_LightVPMatrix;
	glm::vec3 m_LightDir;
};