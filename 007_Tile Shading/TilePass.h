#pragma once
#include "RenderPass.h"
#include <vector>

class CTilePass : public IRenderPass
{
public:
	CTilePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CTilePass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
};