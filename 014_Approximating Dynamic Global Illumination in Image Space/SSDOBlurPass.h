#pragma once
#include "RenderPass.h"
#include <GL/glew.h>


class CSSDOBlurPass : public IRenderPass
{
public:
	CSSDOBlurPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CSSDOBlurPass();

	virtual void initV();
	virtual void updateV();

private:
	int m_FBO;
};