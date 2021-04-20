#include "SSDOBlurPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <random>

CSSDOBlurPass::CSSDOBlurPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CSSDOBlurPass::~CSSDOBlurPass()
{
}

void CSSDOBlurPass::initV()
{
	m_pShader = std::make_shared<CShader>("SSDOBlurPass_VS.glsl", "SSDOBlurPass_FS.glsl");
	auto TextureConfig4Finial = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4Finial->InternalFormat = GL_RGBA32F;
	TextureConfig4Finial->ExternalFormat = GL_RGBA;
	TextureConfig4Finial->DataType = GL_FLOAT;
	TextureConfig4Finial->Width = ElayGraphics::WINDOW_KEYWORD::getWindowWidth();
	TextureConfig4Finial->Height = ElayGraphics::WINDOW_KEYWORD::getWindowHeight();;
	TextureConfig4Finial->Type4WrapS = GL_CLAMP_TO_EDGE;
	TextureConfig4Finial->Type4WrapT = GL_CLAMP_TO_EDGE;
	genTexture(TextureConfig4Finial);

	m_FBO = genFBO({TextureConfig4Finial});

	ElayGraphics::ResourceManager::registerSharedData("BlurSSDOTexture", TextureConfig4Finial);

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_SSDOTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("SSDOTexture"));

}

void CSSDOBlurPass::updateV()
{

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
