#include "RCASPass.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"
#include <GLM/gtc/type_ptr.hpp>

CRCASPass::CRCASPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CRCASPass::~CRCASPass()
{
}

void CRCASPass::initV()
{

	auto TextureConfig4RCAS = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4RCAS->InternalFormat = GL_RGBA32F;
	TextureConfig4RCAS->ExternalFormat = GL_RGBA;
	TextureConfig4RCAS->DataType = GL_FLOAT;
	TextureConfig4RCAS->ImageBindUnit = 1;
	genTexture(TextureConfig4RCAS);
	glm::vec4 Const = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec4>("Const");
	m_pShader = std::make_shared<CShader>("RCASPass_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_RASUTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("EASUTexture"));
	m_pShader->setFloatUniformValue("u_Con0", Const.x, Const.y, Const.z, Const.w);
	m_pShader->setImageUniformValue(TextureConfig4RCAS);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
	ElayGraphics::ResourceManager::registerSharedData("RCASTexture", TextureConfig4RCAS);
}

void CRCASPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	
}