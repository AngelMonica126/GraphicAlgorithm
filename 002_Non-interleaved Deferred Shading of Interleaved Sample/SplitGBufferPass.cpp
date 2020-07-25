#include "SplitGBufferPass.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"

CSplitGBufferPass::CSplitGBufferPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CSplitGBufferPass::~CSplitGBufferPass()
{
}

void CSplitGBufferPass::initV()
{
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
	TextureConfig4Albedo->ImageBindUnit= 0;
	TextureConfig4Normal->ImageBindUnit = 1;
	TextureConfig4Position->ImageBindUnit = 2;
	genTexture(TextureConfig4Albedo);
	genTexture(TextureConfig4Normal);
	genTexture(TextureConfig4Position);

	m_pShader = std::make_shared<CShader>("SplitGBuffer_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_InputAlbedoTexture", AlbedoTexture);
	m_pShader->setTextureUniformValue("u_InputNormalTexture", NormalTexture);
	m_pShader->setTextureUniformValue("u_InputPositionTexture", PositionTexture);
	m_pShader->setIntUniformValue("u_WindowWidth", ElayGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setIntUniformValue("u_WindowHeight", ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setIntUniformValue("u_SubBufferNumX", m_SubBufferNumX);
	m_pShader->setIntUniformValue("u_SubBufferNumY", m_SubBufferNumY);
	m_pShader->setImageUniformValue(TextureConfig4Albedo);
	m_pShader->setImageUniformValue(TextureConfig4Normal);
	m_pShader->setImageUniformValue(TextureConfig4Position);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);

	ElayGraphics::ResourceManager::registerSharedData("SubBufferNumX", m_SubBufferNumX);
	ElayGraphics::ResourceManager::registerSharedData("SubBufferNumY", m_SubBufferNumY);
	ElayGraphics::ResourceManager::registerSharedData("SplitedAlbedoImage", TextureConfig4Albedo);
	ElayGraphics::ResourceManager::registerSharedData("SplitedNormalImage", TextureConfig4Normal);
	ElayGraphics::ResourceManager::registerSharedData("SplitedPositionImage", TextureConfig4Position);
}

void CSplitGBufferPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}