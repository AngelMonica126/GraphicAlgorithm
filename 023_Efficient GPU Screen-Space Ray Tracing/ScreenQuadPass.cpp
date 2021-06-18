#include "ScreenQuadPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>

CScreenQuadPass::CScreenQuadPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CScreenQuadPass::~CScreenQuadPass()
{
}

//************************************************************************************
//Function:
void CScreenQuadPass::initV()
{

	m_pShader = std::make_shared<CShader>("ScreenQuad_VS.glsl", "ScreenQuad_FS.glsl");

	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_Exposure", ElayGraphics::ResourceManager::getSharedDataByName<float>("Exposure"));
	m_pShader->setTextureUniformValue("u_AlbedoTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("AlbedoTexture"));
}

//************************************************************************************
//Function:
void CScreenQuadPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}