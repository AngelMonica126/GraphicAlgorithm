#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

class CFitContinuityPlanePass : public IRenderPass
{
public:
	CFitContinuityPlanePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CFitContinuityPlanePass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	float m_DistThresh = 1.7;
	float m_NormalThresh = 0.9;
	int  m_OldKeyKStatus = -1;
	int  m_OldKeyLStatus = -1;
	int  m_OldKeyHStatus = -1;
	int  m_OldKeyJStatus = -1;
	int m_Block;
};