#include "NonuniformBlurPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"

CNonuniformBlurPass::CNonuniformBlurPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CNonuniformBlurPass::~CNonuniformBlurPass()
{
}

//************************************************************************************
//Function:
void CNonuniformBlurPass::initV()
{
	m_pShader = std::make_shared<CShader>("NonuniformBlur_VS.glsl", "NonuniformBlur_FS.glsl");
	m_FirstTexture = std::make_shared<ElayGraphics::STexture>();
	m_FirstTexture->InternalFormat = GL_RGBA32F;
	m_FirstTexture->ExternalFormat = GL_RGBA;
	m_FirstTexture->DataType = GL_FLOAT;
	m_FirstTexture->isMipmap = true;
	genTexture(m_FirstTexture);

	m_FBO = genFBO({ m_FirstTexture });
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_CoarserTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("AlbedoTexture"));

	ElayGraphics::ResourceManager::registerSharedData("Texture", m_FirstTexture);

}

//************************************************************************************
//Function:
float angle = 0;
void CNonuniformBlurPass::updateV()
{
	genGenerateMipmap(ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("AlbedoTexture"));
	float x = glm::cos(glm::radians(angle)) * 0.5;
	float y = glm::sin(glm::radians(angle)) * 0.5;
	int size = 1;
	for (int i = 10; i >= 0; i--)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_FirstTexture->TextureID, i);
		glViewport(0, 0, size, size);
		m_pShader->activeShader();
		m_pShader->setFloatUniformValue("g_focus", x, y);
		m_pShader->setIntUniformValue("g_level", i);
		m_pShader->setTextureUniformValue("u_SourceTexture", m_FirstTexture);
		m_pShader->setTextureUniformValue("u_CoarserTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("AlbedoTexture"));
		drawQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		size *= 2;
	}
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	angle++;
}