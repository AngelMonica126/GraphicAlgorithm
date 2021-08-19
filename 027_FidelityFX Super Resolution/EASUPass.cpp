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


void CEASUPass::fsrEasuCon(
	glm::vec4& Con0,
	glm::vec4& Con1,
	glm::vec4& Con2,
	glm::vec4& Con3,
	float inputSizeInPixelsX,
	float inputSizeInPixelsY,
	float outputSizeInPixelsX,
	float outputSizeInPixelsY) {
	Con0[0] = (inputSizeInPixelsX * 1.0f / outputSizeInPixelsX);
	Con0[1] = (inputSizeInPixelsY * 1.0f / outputSizeInPixelsY);
	Con0[2] = (0.5f * inputSizeInPixelsX * 1.0f / outputSizeInPixelsX - 0.5f);
	Con0[3] = (0.5f * inputSizeInPixelsY * 1.0f / outputSizeInPixelsY - 0.5f);
	Con1[0] = (1.0f / inputSizeInPixelsX);
	Con1[1] = (1.0f / inputSizeInPixelsY);
	Con1[2] = (1.0 * 1.0f / inputSizeInPixelsX);
	Con1[3] = (-1.0f * 1.0f / inputSizeInPixelsY);
	Con2[0] = (-1.0f * 1.0f / inputSizeInPixelsX);
	Con2[1] = (2.0f * 1.0f / inputSizeInPixelsY);
	Con2[2] = (1.0f * 1.0f / inputSizeInPixelsX);
	Con2[3] = (2.0f * 1.0f / inputSizeInPixelsY);
	Con3[0] = (0.0f * 1.0f / inputSizeInPixelsX);
	Con3[1] = (4.0f * 1.0f / inputSizeInPixelsY);
	Con3[2] = Con3[3] = 0;
}

void CEASUPass::initV()
{
	auto m_InputTexture = std::make_shared<ElayGraphics::STexture>();
	loadTextureFromFile("../Textures/Other/onepiece_medium.png", m_InputTexture);

	m_DisplayWidth =  ElayGraphics::WINDOW_KEYWORD::getWindowWidth();
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
	glm::vec4 Con0 = glm::vec4(0);
	glm::vec4 Con1 = glm::vec4(0);
	glm::vec4 Con2 = glm::vec4(0);
	glm::vec4 Con3 = glm::vec4(0);
	fsrEasuCon(Con0, Con1, Con2, Con3, m_RenderWidth, m_RenderHeight,m_DisplayWidth, m_DisplayHeight);
	m_pShader = std::make_shared<CShader>("EASUPass_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_InputTexture", m_InputTexture);
	m_pShader->setFloatUniformValue("u_Con0", Con0.x, Con0.y, Con0.z, Con0.w);
	m_pShader->setFloatUniformValue("u_Con1", Con1.x, Con1.y, Con1.z, Con1.w);
	m_pShader->setFloatUniformValue("u_Con2", Con2.x, Con2.y, Con2.z, Con2.w);
	m_pShader->setFloatUniformValue("u_Con3", Con3.x, Con3.y, Con3.z, Con3.w);
	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((m_DisplayWidth + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((m_DisplayHeight + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
	ElayGraphics::ResourceManager::registerSharedData("EASUTexture", TextureConfig4EASU);
	ElayGraphics::ResourceManager::registerSharedData("NormalTexture", m_InputTexture);
	ElayGraphics::ResourceManager::registerSharedData("Const", Con0);
}

void CEASUPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}