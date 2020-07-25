#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>

class CHalfCornellBox;

class CRSMBufferPass : public IRenderPass
{
public:
	CRSMBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CRSMBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<CHalfCornellBox> m_pHalfCornellBox;
	int m_FBO = -1;
	int m_RSMResolution = 256;	//1024
	glm::vec3 m_DirectionalLightDirection = glm::vec3(-1, -1, -1);
};