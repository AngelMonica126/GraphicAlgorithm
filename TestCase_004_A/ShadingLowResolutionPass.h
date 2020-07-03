#pragma once
#include "RenderPass.h"
#include <vector>
#include "LightSource.h"
using namespace  std;


class CShadingLowResolutionPass : public IRenderPass
{
public:
	CShadingLowResolutionPass(const std::string& vPassName, int vExcutionOrder, ElayGraphics::ERenderPassType vtype);
	virtual ~CShadingLowResolutionPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	int ssbo;
	size_t LightSourecesByteSize;
	vector<SPointLight>* pLightSources;
	int LightNum;
	int blockx;
	int blocky;
};