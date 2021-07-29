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
	m_pShader->setTextureUniformValue("u_Texture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("DirectIlluminationImage"));
}

//************************************************************************************
//Function:
void CScreenQuadPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_O) == GLFW_PRESS && m_OldKeyDebug != GLFW_PRESS)
	{
		m_OldKeyDebug = GLFW_PRESS;
		m_pShader->setTextureUniformValue("u_Texture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("CSMDebugImage"));
		std::cout << "Debug" << std::endl;
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_O) == GLFW_RELEASE)
		m_OldKeyDebug = GLFW_RELEASE;
	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_PRESS && m_OldKeyNormal != GLFW_PRESS)
	{
		m_OldKeyNormal = GLFW_PRESS;
		m_pShader->setTextureUniformValue("u_Texture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("DirectIlluminationImage"));

		std::cout << "Normal " << std::endl;
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_P) == GLFW_RELEASE)
		m_OldKeyNormal = GLFW_RELEASE;
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}