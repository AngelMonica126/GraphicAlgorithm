#pragma once
#include "RenderPass.h"
#include <GL/glew.h>


class CIndirectShadingPass : public IRenderPass
{
public:
	CIndirectShadingPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CIndirectShadingPass();

	virtual void initV();
	virtual void updateV();

private:
	int m_FBO;
};