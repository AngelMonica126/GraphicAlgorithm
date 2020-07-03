#pragma once
#include "RenderPass.h"
#include <vector>

class CShadingOnSplitedGBufferPass : public IRenderPass
{
public:
	CShadingOnSplitedGBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CShadingOnSplitedGBufferPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
};