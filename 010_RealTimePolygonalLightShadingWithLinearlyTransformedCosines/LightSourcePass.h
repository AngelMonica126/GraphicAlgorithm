#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>

class CLightSourcePass : public IRenderPass
{
public:
	CLightSourcePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CLightSourcePass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	float m_Intensity = 0.0;
};