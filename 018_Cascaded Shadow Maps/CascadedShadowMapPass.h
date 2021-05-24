#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
class CSponza;
class CLightCamera;
class CascadedShadowMapPass : public IRenderPass
{
public:
	CascadedShadowMapPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CascadedShadowMapPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<CSponza> m_pSponza;
	std::shared_ptr<CLightCamera> m_pCLightCamera;
	int m_FBO = -1;
	int m_RSMResolution = 1024;
	int m_SplitNum = 8;
};