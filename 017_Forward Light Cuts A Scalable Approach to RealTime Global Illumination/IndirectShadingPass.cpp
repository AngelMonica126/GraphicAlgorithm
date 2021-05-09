#include "IndirectShadingPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CIndirectShadingPass::CIndirectShadingPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CIndirectShadingPass::~CIndirectShadingPass()
{
}

void CIndirectShadingPass::initV()
{
	
	m_pShader = std::make_shared<CShader>("IndirectShading_VS.glsl", "IndirectShading_FS.glsl");
	auto TextureConfig4IndirectLight = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4IndirectLight->InternalFormat = GL_RGBA32F;
	TextureConfig4IndirectLight->ExternalFormat = GL_RGBA;
	TextureConfig4IndirectLight->DataType = GL_FLOAT;
	genTexture(TextureConfig4IndirectLight);
	m_FBO = genFBO({TextureConfig4IndirectLight});

	ElayGraphics::ResourceManager::registerSharedData("IndirectLightTexture", TextureConfig4IndirectLight);

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_InputNormalTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("NormalTexture"));
	m_pShader->setTextureUniformValue("u_InputPositionTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("PositionTexture"));
	m_pShader->setFloatUniformValue("u_Intensity", ElayGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));

}																

void CIndirectShadingPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}