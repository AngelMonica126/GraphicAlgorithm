#pragma once
#include "RenderPass.h"
#include <vector>

class CEdgeDetectionPass : public IRenderPass
{
public:
	CEdgeDetectionPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CEdgeDetectionPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	float NormalThreshold = 0;
	float DepthThreshold = 1.15f;
	int  m_OldKeyADDNormalThreshold = -1;
	int  m_OldKeyDecreseNormalThreshold = -1;
	int  m_OldKeyADDDepthThreshold = -1;
	int  m_OldKeyDecreseDepthThreshold = -1;
};



