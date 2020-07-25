#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>

class CShadingWithRSMPass : public IRenderPass
{
public:
	CShadingWithRSMPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CShadingWithRSMPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	glm::mat4 m_LightViewMatrix;
	glm::mat4 m_LightProjectionMatrix;
	glm::mat4 m_LightVPMatrix;
	glm::vec3 m_LightDir;
	bool m_EnableIndirectIllumination = true;
	int  m_OldKeyIStatus = -1;
};