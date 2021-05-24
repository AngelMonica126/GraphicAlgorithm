#pragma once
#include "RenderPass.h"

class CNonuniformBlurPass : public IRenderPass
{
public:
	CNonuniformBlurPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CNonuniformBlurPass();

	virtual void initV() override;
	virtual void updateV() override;
private:
	GLint m_FBO;
	std::shared_ptr<ElayGraphics::STexture> m_FirstTexture;
};