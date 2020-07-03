#pragma once
#include "RenderPass.h"
#include <vector>

class CLowResolutionPass : public IRenderPass
{
public:
	CLowResolutionPass(const std::string& vPassName, int vExcutionOrder, ElayGraphics::ERenderPassType vtype);
	virtual ~CLowResolutionPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	int m_LowWidth;
	int m_LowHeight;
};