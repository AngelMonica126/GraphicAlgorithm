#include "ShadingOnSplitedGBufferPass.h"
#include "Interface.h"
#include "Utils.h"
#include "LightSource.h"
#include "Shader.h"

CShadingOnSplitedGBufferPass::CShadingOnSplitedGBufferPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CShadingOnSplitedGBufferPass::~CShadingOnSplitedGBufferPass()
{
}

void CShadingOnSplitedGBufferPass::initV()
{
	auto pLightSources = ElayGraphics::ResourceManager::getSharedDataByName<SPointLight*>("LightSources");
	auto LightSourecesByteSize = ElayGraphics::ResourceManager::getSharedDataByName<size_t>("LightLightSourcesByteSize");
	auto LightNum = ElayGraphics::ResourceManager::getSharedDataByName<size_t>("LightNum");
	genBuffer(GL_SHADER_STORAGE_BUFFER, LightSourecesByteSize, pLightSources, GL_STATIC_DRAW, 0);

	auto TextureConfig = std::make_shared<ElayGraphics::STexture>();;
	TextureConfig->InternalFormat = GL_RGBA32F;
	TextureConfig->ExternalFormat = GL_RGBA;
	TextureConfig->DataType = GL_FLOAT;
	TextureConfig->Type4MinFilter = GL_NEAREST;
	TextureConfig->Type4MagFilter = GL_NEAREST;
	TextureConfig->ImageBindUnit = 0;
	genTexture(TextureConfig);
	ElayGraphics::ResourceManager::registerSharedData("ShadingTexture", TextureConfig);

	m_pShader = std::make_shared<CShader>("ShadingOnSplitedGBuffer_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_TotalLightNum", static_cast<int>(LightNum));
	m_pShader->setIntUniformValue("u_WindowWidth", ElayGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setIntUniformValue("u_WindowHeight", ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setIntUniformValue("u_SubBufferNumX", ElayGraphics::ResourceManager::getSharedDataByName<int>("SubBufferNumX"));
	m_pShader->setIntUniformValue("u_SubBufferNumY", ElayGraphics::ResourceManager::getSharedDataByName<int>("SubBufferNumY"));
	m_pShader->setTextureUniformValue("u_AlbedoTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("SplitedAlbedoImage"));
	m_pShader->setTextureUniformValue("u_NormalTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("SplitedNormalImage"));
	m_pShader->setTextureUniformValue("u_PositionTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("SplitedPositionImage"));
	m_pShader->setImageUniformValue(TextureConfig);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
}

void CShadingOnSplitedGBufferPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}