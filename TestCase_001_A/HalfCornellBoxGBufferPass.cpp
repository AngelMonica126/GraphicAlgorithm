#include "HalfCornellBoxGBufferPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "HalfCornelBox.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CHalfCornellBoxGBufferPass::CHalfCornellBoxGBufferPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CHalfCornellBoxGBufferPass::~CHalfCornellBoxGBufferPass()
{
}

void CHalfCornellBoxGBufferPass::initV()
{
	m_pShader = std::make_shared<CShader>("HalfCornellBox_VS.glsl", "HalfCornellBox_FS.glsl");
	m_pHalfCornellBox = std::dynamic_pointer_cast<CHalfCornellBox>(ElayGraphics::ResourceManager::getGameObjectByName("HalfCornellBox"));
	auto TextureConfig4Position = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4Normal = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4Albedo = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4Depth = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4Albedo->InternalFormat = TextureConfig4Normal->InternalFormat = TextureConfig4Position->InternalFormat = GL_RGBA32F;
	TextureConfig4Albedo->ExternalFormat = TextureConfig4Normal->ExternalFormat = TextureConfig4Position->ExternalFormat = GL_RGBA;
	TextureConfig4Albedo->DataType = TextureConfig4Normal->DataType = TextureConfig4Position->DataType = GL_FLOAT;
	genTexture(TextureConfig4Albedo);
	genTexture(TextureConfig4Normal);
	genTexture(TextureConfig4Position);
	TextureConfig4Depth->InternalFormat = GL_DEPTH_COMPONENT32F;
	TextureConfig4Depth->ExternalFormat = GL_DEPTH_COMPONENT;
	TextureConfig4Depth->DataType = GL_FLOAT;
	TextureConfig4Depth->Type4MinFilter = GL_NEAREST;
	TextureConfig4Depth->Type4MagFilter = GL_NEAREST;
	TextureConfig4Depth->TextureAttachmentType = ElayGraphics::STexture::ETextureAttachmentType::DepthTexture;
	genTexture(TextureConfig4Depth);

	m_FBO = genFBO({ TextureConfig4Albedo,TextureConfig4Normal,TextureConfig4Position,TextureConfig4Depth });

	ElayGraphics::ResourceManager::registerSharedData("AlbedoTexture", TextureConfig4Albedo);
	ElayGraphics::ResourceManager::registerSharedData("NormalTexture", TextureConfig4Normal);
	ElayGraphics::ResourceManager::registerSharedData("PositionTexture", TextureConfig4Position);
	ElayGraphics::ResourceManager::registerSharedData("DepthTexture", TextureConfig4Depth);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pHalfCornellBox->getModelMatrix()));
	m_pHalfCornellBox->initModel(*m_pShader);
}

void CHalfCornellBoxGBufferPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_pShader->activeShader();

	m_pHalfCornellBox->updateModel(*m_pShader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}