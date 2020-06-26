#include "ShadowmapPass.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "Shader.h"
#include <GL/glew.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CShadowmapPass::CShadowmapPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CShadowmapPass::~CShadowmapPass()
{
}

void CShadowmapPass::initV()
{
	m_pShader = std::make_shared<CShader>("Shadowmap_VS.glsl", "Shadowmap_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));
	auto TextureConfig4Color = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4Depth = std::make_shared<ElayGraphics::STexture>();
	genTexture(TextureConfig4Color);
	TextureConfig4Depth->InternalFormat = GL_DEPTH_COMPONENT32F;
	TextureConfig4Depth->ExternalFormat = GL_DEPTH_COMPONENT;
	TextureConfig4Depth->DataType = GL_FLOAT;
	TextureConfig4Depth->Type4MinFilter = GL_LINEAR;
	TextureConfig4Depth->Type4MagFilter = GL_LINEAR;
	TextureConfig4Depth->Width = TextureConfig4Depth->Height = m_ShadowmapResolution;
	TextureConfig4Depth->Type4WrapS = TextureConfig4Depth->Type4WrapT = GL_CLAMP_TO_BORDER;
	TextureConfig4Depth->BorderColor = { 0,0,0,0 };
	TextureConfig4Depth->TextureAttachmentType = ElayGraphics::STexture::ETextureAttachmentType::DepthTexture;
	genTexture(TextureConfig4Depth);

	m_FBO = genFBO({ TextureConfig4Color, TextureConfig4Depth });

	ElayGraphics::ResourceManager::registerSharedData("LightDepthTexture", TextureConfig4Depth);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	glm::mat4 LightProjectionMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
	m_pSponza->initModel(*m_pShader);
}

void CShadowmapPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, m_ShadowmapResolution, m_ShadowmapResolution);

	m_pShader->activeShader();
#ifdef _DEBUG
	glm::mat4 LightProjectionMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	glm::mat4 LightViewMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(LightProjectionMatrix * LightViewMatrix));
#endif
	m_pSponza->updateModel(*m_pShader);

	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}