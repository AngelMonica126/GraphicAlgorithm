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
	m_pShader->setTextureUniformValue("u_ShadingTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("ShadingTexture"));

}

//************************************************************************************
//Function:
void CScreenQuadPass::updateV()
{

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_M) == GLFW_PRESS && m_OldKeyMStatusSave != GLFW_PRESS)
	{
		m_OldKeyMStatusSave = GLFW_PRESS;
		captureScreen2Img("SponzaGlossyToDiffuseGGX.png");
		std::cout << "Save Image" << std::endl;
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_M) == GLFW_RELEASE)
		m_OldKeyMStatusSave = GLFW_RELEASE;

}