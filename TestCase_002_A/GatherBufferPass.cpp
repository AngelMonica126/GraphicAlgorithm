#include "GatherBufferPass.h"
#include "Shader.h"
#include "Utils.h"
#include "Interface.h"

CGatherBufferPass::CGatherBufferPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CGatherBufferPass::~CGatherBufferPass()
{
}

void CGatherBufferPass::initV()
{
	auto TextureConfig = std::make_shared<ElayGraphics::STexture>();;
	TextureConfig->InternalFormat = GL_RGBA32F;
	TextureConfig->ExternalFormat = GL_RGBA;
	TextureConfig->DataType = GL_FLOAT;
	TextureConfig->Type4MinFilter = GL_LINEAR;
	TextureConfig->Type4MagFilter = GL_LINEAR;
	TextureConfig->ImageBindUnit = 0;
	genTexture(TextureConfig);
	ElayGraphics::ResourceManager::registerSharedData("GatherTexture", TextureConfig);
	//ElayGraphics::ResourceManager::registerSharedData("FinalTexture", GatherTexture);

	auto ShadingTexture = ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("ShadingTexture");

	m_pShader = std::make_shared<CShader>("GatherBufferPass_CS.glsl");

	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_WindowWidth", ElayGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setIntUniformValue("u_WindowHeight", ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setIntUniformValue("u_SubBufferNumX", ElayGraphics::ResourceManager::getSharedDataByName<int>("SubBufferNumX"));
	m_pShader->setIntUniformValue("u_SubBufferNumY", ElayGraphics::ResourceManager::getSharedDataByName<int>("SubBufferNumY"));
	m_pShader->setTextureUniformValue("u_LightedSplitedTexture", ShadingTexture);
	m_pShader->setImageUniformValue(TextureConfig);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
}

void CGatherBufferPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}