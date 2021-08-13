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
	m_pShader->setTextureUniformValue("u_InputTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RCASTexture"));
}

//************************************************************************************
//Function:
void CScreenQuadPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_O) == GLFW_PRESS && m_OldKeyNormal != GLFW_PRESS)
	{
		m_OldKeyNormal = GLFW_PRESS;
		m_pShader->setTextureUniformValue("u_InputTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("NormalTexture"));
		std::cout << "Normal "  << std::endl;
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_O) == GLFW_RELEASE)
		m_OldKeyNormal = GLFW_RELEASE;

	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_PRESS && m_OldKeyFSR != GLFW_PRESS)
	{
		m_OldKeyFSR = GLFW_PRESS;
		m_pShader->setTextureUniformValue("u_InputTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RCASTexture"));
		std::cout << "FSR " << std::endl;
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_RELEASE)
		m_OldKeyFSR = GLFW_RELEASE;
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}