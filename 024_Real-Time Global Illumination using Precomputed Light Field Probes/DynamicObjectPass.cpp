#include "DynamicObjectPass.h"
#include "DynamicObject.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CDynamicObjectPass::CDynamicObjectPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CDynamicObjectPass::~CDynamicObjectPass()
{
}

//************************************************************************************
//Function:
void CDynamicObjectPass::initV()
{
	m_pShader = std::make_shared<CShader>("DynamicObject_VS.glsl", "DynamicObject_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<CDynamicObject>(ElayGraphics::ResourceManager::getGameObjectByName("DynamicObject"));

	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_Exposure", ElayGraphics::ResourceManager::getSharedDataByName<float>("Exposure"));
	m_pShader->setTextureUniformValue("u_AlbedoTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("AlbedoTexture"));
	m_pShader->setTextureUniformValue("u_PositionTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("PositionTexture"));
	m_pShader->setTextureUniformValue("u_NormalTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("NormalTexture"));

	m_pShader->setTextureUniformValue("u_OutputOctRadianceImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("OctRadianceTextures"));
	m_pShader->setTextureUniformValue("u_OutputOctNormalImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("OctNormalTextures"));
	m_pShader->setTextureUniformValue("u_OutputOctChebyshevsImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("OctChebyshevsTextures"));
	m_pShader->setTextureUniformValue("u_OutputOctIrradianceImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("OctIrradianceTextures"));
	auto MinAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	auto MaxAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");
	glm::ivec3 CellNum = glm::ivec3((int)MaxAABB.x - (int)MinAABB.x, (int)MaxAABB.y - (int)MinAABB.y, (int)MaxAABB.z - (int)MinAABB.z);
	m_pShader->setFloatUniformValue("u_CellSize", 1.0);
	m_pShader->setFloatUniformValue("u_MinAABB", MinAABB.x, MinAABB.y, MinAABB.z);
	m_pShader->setIntUniformValue("u_CellNum", CellNum.x, CellNum.y, CellNum.z);
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pSponza->initModel(*m_pShader);
}

//************************************************************************************
//Function:
void CDynamicObjectPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_pShader->activeShader();
	//m_pSponza->updateModel(*m_pShader);
	drawSphere();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}