#pragma once
#include "RenderPass.h"
#include <memory>
#include <GL/glew.h>

class CornellBox;

class CGBufferPass : public IRenderPass
{
public:
	CGBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CGBufferPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<CornellBox> m_pSponza;
	GLint m_FBO;
};