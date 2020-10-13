#pragma once
#include "RenderPass.h"
#include <memory>
#include <GL/glew.h>

class CSponza;

class CShadingPass : public IRenderPass
{
public:
	CShadingPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CShadingPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<CSponza> m_pSponza;
	GLint m_FBO;
};