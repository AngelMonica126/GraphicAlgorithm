#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
class CSponza;

class CShadowMapPass : public IRenderPass
{
public:
	CShadowMapPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CShadowMapPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<CSponza> m_pSponza;
	int m_FBO = -1;
	int m_RSMResolution = 512;
};