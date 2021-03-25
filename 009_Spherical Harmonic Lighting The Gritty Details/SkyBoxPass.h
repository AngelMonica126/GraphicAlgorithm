#pragma once
#include "RenderPass.h"
#include <memory>
#include <GL/glew.h>

class CSkyboxPass : public IRenderPass
{
public:
	CSkyboxPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CSkyboxPass();

	virtual void initV() override;
	virtual void updateV() override;
private:
	GLint m_FBO;
};