#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>

class CSponza;

class CRSMBufferPass : public IRenderPass
{
public:
	CRSMBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CRSMBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<CSponza> m_pSponza;
	int m_FBO = -1;
	int m_RSMResolution = 32;	//1024
};