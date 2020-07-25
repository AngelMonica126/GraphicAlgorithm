#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>

struct SVPL
{
	glm::vec4 PositionInViewSpace;
	glm::vec4 NormalInViewSpaceAndAlphai;
	glm::vec4 RadianceIntensityAndRadius;
};

class CCalculateVPLRadiusPass : public IRenderPass
{
public:
	CCalculateVPLRadiusPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CCalculateVPLRadiusPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	float m_MaxErrorBound = 0.0005f;

	void __gengrateRandomNumbers4VPLs();
};