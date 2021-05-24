#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include "Common.h"
class CLightSourcePass : public IRenderPass
{
public:
	CLightSourcePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CLightSourcePass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	float m_Intensity = 0.0;
	int m_FBO = -1;
	int  m_OldKeyPStatus = -1;
	bool m_IsRender = true;
	std::shared_ptr<ElayGraphics::STexture>   m_LightSourceTexture = 0;
	std::vector<unsigned int>vbos;
	std::vector<unsigned int>vaos;
};