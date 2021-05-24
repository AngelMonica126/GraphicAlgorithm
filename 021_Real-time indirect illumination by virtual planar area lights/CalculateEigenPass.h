#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

class CCalculateEigenPass : public IRenderPass
{
public:
	CCalculateEigenPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CCalculateEigenPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	float m_DistThresh = 0.06;
	int  m_OldKeyKStatus = -1;
	int  m_OldKeyLStatus = -1;
};