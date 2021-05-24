#pragma once
#include "RenderPass.h"
#include <vector>
#include "Common.h"
class CGaussianBlurPass : public IRenderPass
{
public:
	CGaussianBlurPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CGaussianBlurPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_PingPongFBO{ 0,0 };
	std::vector<std::shared_ptr<ElayGraphics::STexture>> m_PingPongTexture;
	std::shared_ptr<ElayGraphics::STexture> m_InputTexture = 0;
	std::shared_ptr<ElayGraphics::STexture> m_OutputTexture = 0;
	int m_MaxMipLevel = 10;
	int RSMsize;
};