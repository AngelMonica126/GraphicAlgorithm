#pragma once
#include "RenderPass.h"
#include <GL/glew.h>


class CHBAOBlurPass : public IRenderPass
{
public:
	CHBAOBlurPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CHBAOBlurPass();

	virtual void initV();
	virtual void updateV();

private:
	int m_FBO;
};