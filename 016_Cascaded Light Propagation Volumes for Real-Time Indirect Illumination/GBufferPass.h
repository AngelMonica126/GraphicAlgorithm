#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class CSponza;

class GBufferPass : public IRenderPass
{
public:
	GBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~GBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<CSponza> m_pSponza;
	int m_FBO;
};