#pragma once
#include "RenderPass.h"

class CBlendPass : public IRenderPass
{
public:
	CBlendPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CBlendPass();

	virtual void initV();
	virtual void updateV();
private:
	GLint m_FBO;
};