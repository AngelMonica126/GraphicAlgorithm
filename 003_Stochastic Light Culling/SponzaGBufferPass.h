#pragma once
#include "RenderPass.h"

class CSponza;

class CSponzaGBufferPass : public IRenderPass
{
public:
	CSponzaGBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CSponzaGBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<CSponza> m_pSponza;
	int m_FBO;
};