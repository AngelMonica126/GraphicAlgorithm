#pragma once
#include "RenderPass.h"

class CRecontructPass : public IRenderPass
{
public:
	CRecontructPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CRecontructPass();

	virtual void initV() override;
	virtual void updateV() override;
private:
	GLint m_FBO;
};