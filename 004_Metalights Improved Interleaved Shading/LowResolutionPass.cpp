#include "LowResolutionPass.h"
#include "Interface.h"
#include "Utils.h"
#include "LightSource.h"
#include "Shader.h"

CLowResolutionPass::CLowResolutionPass(const std::string& vPassName, int vExcutionOrder , ElayGraphics::ERenderPassType vtype) : IRenderPass(vPassName, vExcutionOrder, vtype)
{
}

CLowResolutionPass::~CLowResolutionPass()
{
}

void CLowResolutionPass::initV()
{
	m_LowWidth = ElayGraphics::WINDOW_KEYWORD::getWindowWidth() / 16;
	m_LowHeight = ElayGraphics::WINDOW_KEYWORD::getWindowHeight() / 16;
	auto AlbedoTexture = ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("AlbedoTexture");
	auto NormalTexture = ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("NormalTexture");
	auto PositionTexture = ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("PositionTexture");

	auto TextureConfig4Albedo = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4Normal = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4Position = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4Albedo->InternalFormat = TextureConfig4Normal->InternalFormat = TextureConfig4Position->InternalFormat = GL_RGBA32F;
	TextureConfig4Albedo->ExternalFormat = TextureConfig4Normal->ExternalFormat = TextureConfig4Position->ExternalFormat = GL_RGBA;
	TextureConfig4Albedo->DataType = TextureConfig4Normal->DataType = TextureConfig4Position->DataType = GL_FLOAT;
	TextureConfig4Albedo->Type4MinFilter = TextureConfig4Normal->Type4MinFilter = TextureConfig4Position->Type4MinFilter = GL_LINEAR;
	TextureConfig4Albedo->Type4MagFilter = TextureConfig4Normal->Type4MagFilter = TextureConfig4Position->Type4MagFilter = GL_LINEAR;
	TextureConfig4Albedo->ImageBindUnit = 0;
	TextureConfig4Normal->ImageBindUnit = 1;
	TextureConfig4Position->ImageBindUnit = 2;
	TextureConfig4Normal->Width = TextureConfig4Position->Width = TextureConfig4Albedo->Width = m_LowWidth;
	TextureConfig4Normal->Height =TextureConfig4Position->Height = TextureConfig4Albedo->Height = m_LowHeight;
	genTexture(TextureConfig4Albedo);
	genTexture(TextureConfig4Normal);
	genTexture(TextureConfig4Position);

	m_pShader = std::make_shared<CShader>("LowResolution_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_WindowWidth", ElayGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setIntUniformValue("u_WindowHeight", ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setIntUniformValue("LowWidth", m_LowWidth);
	m_pShader->setIntUniformValue("LowHeight", m_LowHeight);
	m_pShader->setTextureUniformValue("u_InputAlbedoTexture", AlbedoTexture);
	m_pShader->setTextureUniformValue("u_InputNormalTexture", NormalTexture);
	m_pShader->setTextureUniformValue("u_InputPositionTexture", PositionTexture);
	m_pShader->setImageUniformValue(TextureConfig4Albedo);
	m_pShader->setImageUniformValue(TextureConfig4Normal);
	m_pShader->setImageUniformValue(TextureConfig4Position);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);

	ElayGraphics::ResourceManager::registerSharedData("LowWidth", m_LowWidth);
	ElayGraphics::ResourceManager::registerSharedData("LowHeight", m_LowHeight);
	ElayGraphics::ResourceManager::registerSharedData("LowAlbedoImage", TextureConfig4Albedo);
	ElayGraphics::ResourceManager::registerSharedData("LowNormalImage", TextureConfig4Normal);
	ElayGraphics::ResourceManager::registerSharedData("LowPositionImage", TextureConfig4Position);
}

void CLowResolutionPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}