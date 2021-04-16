#pragma once
#include "RenderPass.h"

class CMergeShadingPass : public IRenderPass
{
public:
	CMergeShadingPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CMergeShadingPass();

	virtual void initV();
	virtual void updateV();
private:
	GLint m_FBO;
};