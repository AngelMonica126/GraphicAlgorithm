#pragma once
#include "RenderPass.h"
#include <vector>

class CSplitGBufferPass : public IRenderPass
{
public:
	CSplitGBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CSplitGBufferPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	int m_SubBufferNumX = 4;
	int m_SubBufferNumY = 4;
};