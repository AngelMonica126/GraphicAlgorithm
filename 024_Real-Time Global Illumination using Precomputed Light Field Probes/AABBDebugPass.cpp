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
	m_Min = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	m_Max = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");
	auto AlbedoTextures = ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("OctChebyshevsTextures");

	m_pShader = std::make_shared<CShader>("AABBDebug_VS.glsl", "AABBDebug_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_OctRadianceTextures", AlbedoTextures);
	m_pShader->setIntUniformValue("u_Index", 0);
}

//************************************************************************************
//Function:
void CAABBDebugPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	m_pShader->activeShader();
	int Index = 0;
	for (int i = m_Min.x; i < m_Max.x; i++)
		for (int j = m_Min.y; j < m_Max.y; j++)
			for (int k = m_Min.z; k < m_Max.z; k++)
			{
				glm::mat4 Model = glm::mat4(1);
				glm::vec3 Position = glm::vec3(i, j, k);
				Model = glm::translate(Model, Position);
				Model = glm::scale(Model, glm::vec3(0.1, 0.1, 0.1));
				m_pShader->setFloatUniformValue("u_LightPos", Position.x, Position.y, Position.z);
				m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(Model));
				m_pShader->setIntUniformValue("u_Index", Index);
				drawSphere();
				Index++;
			}
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}