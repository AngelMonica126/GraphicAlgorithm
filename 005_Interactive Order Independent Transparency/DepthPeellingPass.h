#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class CSponza;

class DepthPeellingPass : public IRenderPass
{
public:
	DepthPeellingPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~DepthPeellingPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<CShader> m_BlendShader;
	std::shared_ptr<CSponza> m_pSponza;
	std::vector<std::shared_ptr<ElayGraphics::STexture>> m_TextureConfig4Color;
	std::vector<std::shared_ptr<ElayGraphics::STexture>> m_TextureConfig4Depth;
	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig;
	std::vector<int> m_FBO;
	int m_BlendFBO = -1;
	int m_Layer = 4;
};