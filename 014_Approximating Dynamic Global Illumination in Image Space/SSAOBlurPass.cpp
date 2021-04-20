#include "SSAOBlurPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <random>

CSSAOBlurPass::CSSAOBlurPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CSSAOBlurPass::~CSSAOBlurPass()
{
}

void CSSAOBlurPass::initV()
{
	m_pShader = std::make_shared<CShader>("SSAOBlurPass_VS.glsl", "SSAOBlurPass_FS.glsl");
	auto TextureConfig4Finial = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4Finial->InternalFormat = GL_RED;
	TextureConfig4Finial->ExternalFormat = GL_RGB;
	TextureConfig4Finial->DataType = GL_FLOAT;
	TextureConfig4Finial->Width = ElayGraphics::WINDOW_KEYWORD::getWindowWidth();
	TextureConfig4Finial->Height = ElayGraphics::WINDOW_KEYWORD::getWindowHeight();;
	TextureConfig4Finial->Type4WrapS = GL_CLAMP_TO_EDGE;
	TextureConfig4Finial->Type4WrapT = GL_CLAMP_TO_EDGE;
	genTexture(TextureConfig4Finial);

	m_FBO = genFBO({TextureConfig4Finial});

	ElayGraphics::ResourceManager::registerSharedData("BlurSSAOTexture", TextureConfig4Finial);

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_SSAOTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("SSAOTexture"));

}

void CSSAOBlurPass::updateV()
{

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
