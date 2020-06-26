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
	auto TextureConfig4Position = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4Normal = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4RandianceIntensity = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4RandianceIntensity->InternalFormat = TextureConfig4Normal->InternalFormat = TextureConfig4Position->InternalFormat = GL_RGBA32F;
	TextureConfig4RandianceIntensity->ExternalFormat = TextureConfig4Normal->ExternalFormat = TextureConfig4Position->ExternalFormat = GL_RGBA;
	TextureConfig4RandianceIntensity->DataType = TextureConfig4Normal->DataType = TextureConfig4Position->DataType = GL_FLOAT;
	TextureConfig4RandianceIntensity->Width = TextureConfig4Normal->Width = TextureConfig4Position->Width = m_RSMResolution;
	TextureConfig4RandianceIntensity->Height = TextureConfig4Normal->Height = TextureConfig4Position->Height = m_RSMResolution;
	TextureConfig4RandianceIntensity->Type4WrapS = TextureConfig4Normal->Type4WrapS = TextureConfig4Position->Type4WrapS = GL_CLAMP_TO_BORDER;
	TextureConfig4RandianceIntensity->Type4WrapT = TextureConfig4Normal->Type4WrapT = TextureConfig4Position->Type4WrapT = GL_CLAMP_TO_BORDER;
	TextureConfig4RandianceIntensity->BorderColor = TextureConfig4Normal->BorderColor = TextureConfig4Position->BorderColor = { 0,0,0,0 };
	genTexture(TextureConfig4RandianceIntensity);
	genTexture(TextureConfig4Normal);
	genTexture(TextureConfig4Position);

	m_FBO = genFBO({ TextureConfig4RandianceIntensity,TextureConfig4Normal,TextureConfig4Position});

	ElayGraphics::ResourceManager::registerSharedData("RSMRandianceIntensityTexture", TextureConfig4RandianceIntensity);
	ElayGraphics::ResourceManager::registerSharedData("RSMNormalTexture", TextureConfig4Normal);
	ElayGraphics::ResourceManager::registerSharedData("RSMPositionTexture", TextureConfig4Position);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	glm::mat4 LightProjectionMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	auto LightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
	m_pShader->setIntUniformValue("u_VPLsCount", m_RSMResolution*m_RSMResolution);
	m_pShader->setFloatUniformValue("u_RSMCameraAreaInWorldSpace", ElayGraphics::ResourceManager::getSharedDataByName<float>("LightCameraAreaInWorldSpace"));

	m_pSponza->initModel(*m_pShader);

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
#ifdef _DEBUG
	glm::mat4 LightProjectionMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	auto LightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
#endif
	m_pSponza->updateModel(*m_pShader);

	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}