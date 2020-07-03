#pragma once
#include "RenderPass.h"
#include "Common.h"
#include <vector>

class CGaussianBlurPass : public IRenderPass
{
public:
	CGaussianBlurPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CGaussianBlurPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int  m_Iterations = 1;
	int  m_IsBlurVerticalUniformId = -1;
	int  m_BlurInputTextureUniformId = -1;
	std::vector<int> m_GlobalGroupSize;
	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig1;
	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig2;
};