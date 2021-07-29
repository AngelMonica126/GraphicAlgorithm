#include "GBufferPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "LightCamera.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

GBufferPass::GBufferPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

GBufferPass::~GBufferPass()
{
}

void GBufferPass::initV()
{
	auto m_pCLightCamera = std::dynamic_pointer_cast<CLightCamera>(ElayGraphics::ResourceManager::getGameObjectByName("LightCamera"));

	m_pShader = std::make_shared<CShader>("GBuffer_VS.glsl", "GBuffer_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));
	auto TextureConfig4Position = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4NormalAndDoubleRoughness = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4AlbedoAndMetallic = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4AlbedoAndMetallic->InternalFormat = TextureConfig4NormalAndDoubleRoughness->InternalFormat = TextureConfig4Position->InternalFormat = GL_RGBA32F;
	TextureConfig4AlbedoAndMetallic->ExternalFormat = TextureConfig4NormalAndDoubleRoughness->ExternalFormat = TextureConfig4Position->ExternalFormat = GL_RGBA;
	TextureConfig4AlbedoAndMetallic->DataType = TextureConfig4NormalAndDoubleRoughness->DataType = TextureConfig4Position->DataType = GL_FLOAT;
	TextureConfig4AlbedoAndMetallic->isMipmap = TextureConfig4NormalAndDoubleRoughness->isMipmap = TextureConfig4Position->isMipmap = false;
	genTexture(TextureConfig4AlbedoAndMetallic);
	genTexture(TextureConfig4NormalAndDoubleRoughness);
	genTexture(TextureConfig4Position);
	m_FBO = genFBO({ TextureConfig4AlbedoAndMetallic, TextureConfig4NormalAndDoubleRoughness, TextureConfig4Position});

	ElayGraphics::ResourceManager::registerSharedData("AlbedoTexture", TextureConfig4AlbedoAndMetallic);
	ElayGraphics::ResourceManager::registerSharedData("NormalTexture", TextureConfig4NormalAndDoubleRoughness);
	ElayGraphics::ResourceManager::registerSharedData("PositionTexture", TextureConfig4Position);
	
	std::vector<glm::mat4> Frustum = m_pCLightCamera->getFrustum();

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pShader->setFloatUniformValue("u_Near", ElayGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Far", ElayGraphics::Camera::getMainCameraFar());
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(Frustum[3] * ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix")));
	m_pShader->setMat4UniformValue("u_TransposeInverseViewModelMatrix", glm::value_ptr(glm::transpose(glm::inverse(ElayGraphics::Camera::getMainCameraViewMatrix() *  m_pSponza->getModelMatrix()))));
	m_pSponza->initModel(*m_pShader);
}

void GBufferPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_TransposeInverseViewModelMatrix", glm::value_ptr(glm::transpose(glm::inverse(ElayGraphics::Camera::getMainCameraViewMatrix() *  m_pSponza->getModelMatrix()))));
	m_pSponza->updateModel(*m_pShader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}