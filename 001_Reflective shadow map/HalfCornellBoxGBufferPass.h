#pragma once
#include "RenderPass.h"

class CHalfCornellBox;

class CHalfCornellBoxGBufferPass : public IRenderPass
{
public:
	CHalfCornellBoxGBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CHalfCornellBoxGBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<CHalfCornellBox> m_pHalfCornellBox;
	int m_FBO;
};