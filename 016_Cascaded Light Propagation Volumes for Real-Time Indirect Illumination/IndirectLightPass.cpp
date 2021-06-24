#include "IndirectLightPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CIndirectLightPass::CIndirectLightPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CIndirectLightPass::~CIndirectLightPass()
{
}

void CIndirectLightPass::initV()
{
	
	m_pShader = std::make_shared<CShader>("IndirectLightPass_VS.glsl", "IndirectLightPass_FS.glsl");
	auto TextureConfig4IndirectLight = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4IndirectLight->InternalFormat = GL_RGBA32F;
	TextureConfig4IndirectLight->ExternalFormat = GL_RGBA;
	TextureConfig4IndirectLight->DataType = GL_FLOAT;
	TextureConfig4IndirectLight->isMipmap = false;
	genTexture(TextureConfig4IndirectLight);
	m_FBO = genFBO({ TextureConfig4IndirectLight });

	ElayGraphics::ResourceManager::registerSharedData("IndirectLightTexture", TextureConfig4IndirectLight);
	glm::vec3 MinAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_CellSize", 1.0);
	m_pShader->setFloatUniformValue("u_MinAABB", MinAABB.x, MinAABB.y, MinAABB.z);
	m_pShader->setTextureUniformValue("u_RAccumulatorLPV", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVAccumulatorRTexture"));
	m_pShader->setTextureUniformValue("u_GAccumulatorLPV", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVAccumulatorGTexture"));
	m_pShader->setTextureUniformValue("u_BAccumulatorLPV", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVAccumulatorBTexture"));
	m_pShader->setTextureUniformValue("u_NormalTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("NormalTexture"));
	m_pShader->setTextureUniformValue("u_PositionTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("PositionTexture"));

}

void CIndirectLightPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShader->activeShader();
	auto ViewMatrix = ElayGraphics::Camera::getMainCameraViewMatrix();
	m_pShader->setMat4UniformValue("u_InverseCameraViewMatrix", glm::value_ptr(glm::inverse(ViewMatrix)));
	auto LightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");


	drawQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}