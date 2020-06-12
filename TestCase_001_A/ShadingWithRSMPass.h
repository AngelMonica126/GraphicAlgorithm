#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>

class CShadingWithRSMPass : public IRenderPass
{
public:
	CShadingWithRSMPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CShadingWithRSMPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	std::vector<glm::vec4> m_VPLsSampleCoordsAndWeights;
	int m_VPLNum = 32;
	float m_MaxSampleRadius = 25;
	glm::mat4 m_LightVPMatrix;
	glm::vec4 m_LightDir;

	void __initVPLsSampleCoordsAndWeights();
};