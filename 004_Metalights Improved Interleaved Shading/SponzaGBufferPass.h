#pragma once
#include "RenderPass.h"
#include <memory>
#include <GL/glew.h>

class CSponza;

class CSponzaGBufferPass : public IRenderPass
{
public:
	CSponzaGBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CSponzaGBufferPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<CSponza> m_pSponza;
	GLint m_FBO;
};