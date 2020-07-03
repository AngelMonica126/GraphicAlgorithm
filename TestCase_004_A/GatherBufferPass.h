#pragma once
#include "RenderPass.h"
#include <vector>

class CGatherBufferPass : public IRenderPass
{
public:
	CGatherBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CGatherBufferPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
};