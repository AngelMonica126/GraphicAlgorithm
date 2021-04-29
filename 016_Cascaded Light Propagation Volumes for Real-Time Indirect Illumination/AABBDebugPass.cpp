#include "AABBDebugPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include "Sponza.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
CAABBDebugPass::CAABBDebugPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CAABBDebugPass::~CAABBDebugPass()
{
}

//************************************************************************************
//Function:
void CAABBDebugPass::initV()
{
	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_Min = m_pSponza->getMin();
	m_Max = m_pSponza->getMax();

	m_pShader = std::make_shared<CShader>("AABBDebug_VS.glsl", "AABBDebug_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_CellSize", 1.0);
	m_pShader->setFloatUniformValue("u_MinAABB", m_Min.x, m_Min.y, m_Min.z);
	m_pShader->setTextureUniformValue("u_RAccumulatorLPV", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVAccumulatorRTexture"));
	m_pShader->setTextureUniformValue("u_GAccumulatorLPV", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVAccumulatorGTexture"));
	m_pShader->setTextureUniformValue("u_BAccumulatorLPV", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVAccumulatorBTexture"));
}

//************************************************************************************
//Function:
void CAABBDebugPass::updateV()
{
	glm::ivec3 begin = m_Max - m_Min;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	m_pShader->activeShader();
	for (int i = m_Min.x; i < m_Max.x; i++)
		for (int j = m_Min.y; j < m_Max.y; j++)
			for (int k = m_Min.z; k < m_Max.z; k++)
			{
				glm::mat4 model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(i,j,k));
				model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
				m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(model));
				drawSphere();
			}
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}