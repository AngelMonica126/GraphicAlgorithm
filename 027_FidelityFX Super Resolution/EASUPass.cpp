#include "EASUPass.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"
#include <GLM/gtc/type_ptr.hpp>

CEASUPass::CEASUPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CEASUPass::~CEASUPass()
{
}

void CEASUPass::initV()
{
	auto m_InputTexture = std::make_shared<ElayGraphics::STexture>();
	loadTextureFromFile("../Textures/Other/onepiece_low.png", m_InputTexture);

	m_DisplayWidth = ElayGraphics::WINDOW_KEYWORD::getWindowWidth();
	m_DisplayHeight = ElayGraphics::WINDOW_KEYWORD::getWindowHeight();

	m_RenderWidth = m_InputTexture->Width;
	m_RenderHeight = m_InputTexture->Height;


	auto TextureConfig4EASU = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4EASU->InternalFormat = GL_RGBA32F;
	TextureConfig4EASU->ExternalFormat = GL_RGBA;
	TextureConfig4EASU->DataType = GL_FLOAT;
	TextureConfig4EASU->ImageBindUnit = 0;
	TextureConfig4EASU->Width = m_DisplayWidth;
	TextureConfig4EASU->Height = m_DisplayHeight;
	genTexture(TextureConfig4EASU);


	m_pShader = std::make_shared<CShader>("EASUPass_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_InputTexture", m_InputTexture);
	m_pShader->setIntUniformValue("u_DisplayWidth", m_DisplayWidth);
	m_pShader->setIntUniformValue("u_DisplayHeight", m_DisplayHeight);
	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((m_DisplayWidth + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((m_DisplayHeight + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
	ElayGraphics::ResourceManager::registerSharedData("EASUTexture", TextureConfig4EASU);
}

void CEASUPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	
}