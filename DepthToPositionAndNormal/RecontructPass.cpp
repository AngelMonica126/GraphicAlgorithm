#include "RecontructPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"

CRecontructPass::CRecontructPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CRecontructPass::~CRecontructPass()
{
}

//************************************************************************************
//Function:
void CRecontructPass::initV()
{
	m_pShader = std::make_shared<CShader>("Recontruct_VS.glsl", "Recontruct_FS.glsl");
	auto TextureConfig4 = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4->InternalFormat = GL_RGBA32F;
	TextureConfig4->ExternalFormat = GL_RGBA;
	TextureConfig4->DataType = GL_FLOAT;
	genTexture(TextureConfig4);


	m_FBO = genFBO({ TextureConfig4 });

	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_WindowWidth", ElayGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setFloatUniformValue("u_WindowHeight", ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setFloatUniformValue("u_Near", ElayGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Fov", glm::radians(ElayGraphics::Camera::getMainCameraFov()));
	m_pShader->setFloatUniformValue("u_Far", ElayGraphics::Camera::getMainCameraFar());
	m_pShader->setTextureUniformValue("u_DepthTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("DepthTexture"));
	m_pShader->setTextureUniformValue("u_PositionTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("PositionTexture"));
	m_pShader->setTextureUniformValue("u_NormalTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("NormalTexture"));
}

//************************************************************************************
//Function:
void CRecontructPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}