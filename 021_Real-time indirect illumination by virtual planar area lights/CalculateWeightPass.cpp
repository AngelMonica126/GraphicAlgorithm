#include "CalculateWeightPass.h"
#include "Utils.h"
#include "Interface.h"
#include "Shader.h"
#include <random>
#include <GLM/gtc/type_ptr.hpp>
#include "vector"
#include <GLFW/glfw3.h>
using namespace std;
CCalculateWeight::CCalculateWeight(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CCalculateWeight::~CCalculateWeight()
{
}

void CCalculateWeight::initV()
{
	int block = ElayGraphics::ResourceManager::getSharedDataByName<int>("Block");
	int resolution = ElayGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");

	m_TextureConfig4AlbedoAndMetallic = std::make_shared<ElayGraphics::STexture>();
	m_TextureConfig4AlbedoAndMetallic->InternalFormat = GL_RGBA32F;
	m_TextureConfig4AlbedoAndMetallic->ExternalFormat = GL_RGBA;
	m_TextureConfig4AlbedoAndMetallic->DataType = GL_FLOAT;
	m_TextureConfig4AlbedoAndMetallic->Type4MinFilter = GL_LINEAR;
	m_TextureConfig4AlbedoAndMetallic->Type4MagFilter = GL_LINEAR;
	m_TextureConfig4AlbedoAndMetallic->ImageBindUnit = 0;
	m_TextureConfig4AlbedoAndMetallic->isMipmap = true;
	m_TextureConfig4AlbedoAndMetallic->Height = m_TextureConfig4AlbedoAndMetallic->Width = resolution;
	genTexture(m_TextureConfig4AlbedoAndMetallic);

	m_pShader = std::make_shared<CShader>("CalculateWeight_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_VALCount", block * block);
	m_pShader->setIntUniformValue("u_Block", block);
	m_pShader->setIntUniformValue("u_RSMResolution", ElayGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution"));
	m_pShader->setTextureUniformValue("u_RSMAlbedoAndMetallicTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RSMAlbedoAndMetallicTexture"));
	m_pShader->setTextureUniformValue("u_RSMPositionTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RSMPositionTexture"));
	m_pShader->setTextureUniformValue("u_RSMNormalTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RSMNormalAndDoubleRoughnessTexture"));
	m_pShader->setImageUniformValue(m_TextureConfig4AlbedoAndMetallic);
	
	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((resolution + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((resolution + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
	ElayGraphics::ResourceManager::registerSharedData("WeightAlbedoImage", m_TextureConfig4AlbedoAndMetallic);
}

void CCalculateWeight::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	genGenerateMipmap(m_TextureConfig4AlbedoAndMetallic);
	
}
