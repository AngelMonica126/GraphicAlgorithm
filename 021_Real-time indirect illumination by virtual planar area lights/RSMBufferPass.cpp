#include "RSMBufferPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "AABB.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CRSMBufferPass::CRSMBufferPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CRSMBufferPass::~CRSMBufferPass()
{
}

void CRSMBufferPass::initV()
{
	m_pShader = std::make_shared<CShader>("RSMBuffer_VS.glsl", "RSMBuffer_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_pDynamicObjectShader = std::make_shared<CShader>("RSMBufferDynamicObject_VS.glsl", "RSMBufferDynamicObject_FS.glsl");
	m_pDynamicObject = std::dynamic_pointer_cast<CDynamicObject>(ElayGraphics::ResourceManager::getGameObjectByName("DynamicObject"));
	auto TextureConfig4Position = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4NormalAndDoubleRoughness = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4RadiantFlux = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4AlbedoAndMetallic = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4Depth = std::make_shared<ElayGraphics::STexture>(); ;
	TextureConfig4AlbedoAndMetallic->InternalFormat = TextureConfig4RadiantFlux->InternalFormat = TextureConfig4NormalAndDoubleRoughness->InternalFormat = TextureConfig4Position->InternalFormat = GL_RGBA32F;
	TextureConfig4AlbedoAndMetallic->ExternalFormat = TextureConfig4RadiantFlux->ExternalFormat = TextureConfig4NormalAndDoubleRoughness->ExternalFormat = TextureConfig4Position->ExternalFormat = GL_RGBA;
	TextureConfig4AlbedoAndMetallic->DataType = TextureConfig4RadiantFlux->DataType = TextureConfig4NormalAndDoubleRoughness->DataType = TextureConfig4Position->DataType = GL_FLOAT;
	TextureConfig4AlbedoAndMetallic->Width = TextureConfig4RadiantFlux->Width = TextureConfig4NormalAndDoubleRoughness->Width = TextureConfig4Position->Width = m_RSMResolution;
	TextureConfig4AlbedoAndMetallic->Height = TextureConfig4RadiantFlux->Height = TextureConfig4NormalAndDoubleRoughness->Height = TextureConfig4Position->Height = m_RSMResolution;
	TextureConfig4AlbedoAndMetallic->Type4WrapS = TextureConfig4RadiantFlux->Type4WrapS = TextureConfig4NormalAndDoubleRoughness->Type4WrapS = GL_CLAMP_TO_EDGE;
	TextureConfig4AlbedoAndMetallic->Type4WrapT = TextureConfig4RadiantFlux->Type4WrapT = TextureConfig4NormalAndDoubleRoughness->Type4WrapT = GL_CLAMP_TO_EDGE;
	TextureConfig4Depth->InternalFormat = GL_DEPTH_COMPONENT32F;
	TextureConfig4Depth->ExternalFormat = GL_DEPTH_COMPONENT;
	TextureConfig4Depth->DataType = GL_FLOAT;
	TextureConfig4Depth->Width = TextureConfig4Depth->Height = m_RSMResolution;
	TextureConfig4Depth->BorderColor = { 0,0,0,0 };
	TextureConfig4Depth->TextureAttachmentType = ElayGraphics::STexture::ETextureAttachmentType::DepthTexture;

	genTexture(TextureConfig4RadiantFlux);
	genTexture(TextureConfig4NormalAndDoubleRoughness);
	genTexture(TextureConfig4Position);
	genTexture(TextureConfig4AlbedoAndMetallic);
	genTexture(TextureConfig4Depth);
	m_FBO = genFBO({ TextureConfig4RadiantFlux,TextureConfig4NormalAndDoubleRoughness,TextureConfig4Position, TextureConfig4AlbedoAndMetallic,TextureConfig4Depth });

	ElayGraphics::ResourceManager::registerSharedData("RSMRadiantFluxTexture", TextureConfig4RadiantFlux);
	ElayGraphics::ResourceManager::registerSharedData("RSMNormalAndDoubleRoughnessTexture", TextureConfig4NormalAndDoubleRoughness);
	ElayGraphics::ResourceManager::registerSharedData("RSMPositionTexture", TextureConfig4Position);
	ElayGraphics::ResourceManager::registerSharedData("RSMAlbedoAndMetallicTexture", TextureConfig4AlbedoAndMetallic);
	ElayGraphics::ResourceManager::registerSharedData("LightDepthTexture", TextureConfig4Depth);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	glm::mat4 LightProjectionMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	auto LightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
	m_pShader->setIntUniformValue("u_VPLsCount", m_RSMResolution * m_RSMResolution);
	m_pShader->setFloatUniformValue("u_RSMCameraAreaInWorldSpace", ElayGraphics::ResourceManager::getSharedDataByName<float>("LightCameraAreaInWorldSpace"));
	m_pShader->setFloatUniformValue("u_Intensity", ElayGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));
	m_pSponza->initModel(*m_pShader);


	m_pDynamicObjectShader->activeShader();
	//m_pDynamicObjectShader->setTextureUniformValue("u_Texture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("InputTexture"));
	m_pDynamicObjectShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pDynamicObject->getModelMatrix()));
	m_pDynamicObjectShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	m_pDynamicObjectShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
	m_pDynamicObjectShader->setIntUniformValue("u_VPLsCount", m_RSMResolution*m_RSMResolution);
	m_pDynamicObjectShader->setFloatUniformValue("u_RSMCameraAreaInWorldSpace", ElayGraphics::ResourceManager::getSharedDataByName<float>("LightCameraAreaInWorldSpace"));
	m_pDynamicObjectShader->setFloatUniformValue("u_Intensity", ElayGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));
	m_pDynamicObject->initModel(*m_pDynamicObjectShader);
	ElayGraphics::ResourceManager::registerSharedData("RSMResolution", m_RSMResolution);
}

void CRSMBufferPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, m_RSMResolution, m_RSMResolution);

	m_pShader->activeShader();
	glm::mat4 LightProjectionMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	m_pSponza->updateModel(*m_pShader);

	m_pDynamicObjectShader->activeShader();
	m_pDynamicObjectShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pDynamicObject->getModelMatrix()));
	m_pDynamicObjectShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	m_pDynamicObject->updateModel(*m_pDynamicObjectShader);

	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}