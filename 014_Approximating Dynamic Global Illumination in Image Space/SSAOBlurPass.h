#pragma once
#include "RenderPass.h"
#include <GL/glew.h>


class CSSAOBlurPass : public IRenderPass
{
public:
	CSSAOBlurPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CSSAOBlurPass();

	virtual void initV();
	virtual void updateV();

private:
	int m_FBO;
};