#include "OpacityShadingPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

COpacityShadingPass::COpacityShadingPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

COpacityShadingPass::~COpacityShadingPass()
{
}

void COpacityShadingPass::initV()
{


	ElayGraphics::Camera::setMainCameraFarPlane(100);
	ElayGraphics::Camera::setMainCameraMoveSpeed(1.0);
	ElayGraphics::Camera::setMainCameraPos({ 0.0484748, -0.932904, -2.94066 });
	ElayGraphics::Camera::setMainCameraFront({ 0.0811415, -0.153331, 0.984838 });
	m_pShader = std::make_shared<CShader>("OpacityShading_VS.glsl", "OpacityShading_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));

	auto TextureConfig4Albedo = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4Depth = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4Albedo->InternalFormat  = GL_RGBA32F;
	TextureConfig4Albedo->ExternalFormat  = GL_RGBA;
	TextureConfig4Albedo->DataType  = GL_FLOAT;
	genTexture(TextureConfig4Albedo);
	TextureConfig4Depth->InternalFormat = GL_DEPTH_COMPONENT32F;
	TextureConfig4Depth->ExternalFormat = GL_DEPTH_COMPONENT;
	TextureConfig4Depth->DataType = GL_FLOAT;
	TextureConfig4Depth->Type4MinFilter = GL_NEAREST;
	TextureConfig4Depth->Type4MagFilter = GL_NEAREST;
	TextureConfig4Depth->TextureAttachmentType = ElayGraphics::STexture::ETextureAttachmentType::DepthTexture;
	genTexture(TextureConfig4Depth);

	m_FBO = genFBO({ TextureConfig4Albedo,TextureConfig4Depth });

	ElayGraphics::ResourceManager::registerSharedData("OpacityAlbedoTexture", TextureConfig4Albedo);
	ElayGraphics::ResourceManager::registerSharedData("OpacityDepthTexture", TextureConfig4Depth);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pSponza->initModel(*m_pShader);

}


void COpacityShadingPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_pShader->activeShader();
	m_pSponza->updateModel(*m_pShader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}