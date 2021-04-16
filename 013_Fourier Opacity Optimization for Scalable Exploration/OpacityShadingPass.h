#pragma once
#include "RenderPass.h"

class CSponza;

class COpacityShadingPass : public IRenderPass
{
public:
	COpacityShadingPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~COpacityShadingPass();

	virtual void initV();
	virtual void updateV();
private:
	int m_FBO = -1;
	std::shared_ptr<CSponza> m_pSponza;
};