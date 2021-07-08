#include "OctahedronPass.h"
#include "Common.h"
#include "Interface.h"
#include "Utils.h"
#include "Shader.h"
#include <random>
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

COctahedronPass::COctahedronPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

COctahedronPass::~COctahedronPass()
{
}

void COctahedronPass::initV()
{
	int BakeResolution = ElayGraphics::ResourceManager::getSharedDataByName<int>("BakeResolution");
	auto BakeAlbedoTextures = ElayGraphics::ResourceManager::getSharedDataByName<std::vector<std::shared_ptr<ElayGraphics::STexture>>>("BakeAlbedoTextures");
	auto BakeNormalTextures = ElayGraphics::ResourceManager::getSharedDataByName<std::vector<std::shared_ptr<ElayGraphics::STexture>>>("BakeNormalTextures");
	auto BakeChebyshevsTextures = ElayGraphics::ResourceManager::getSharedDataByName<std::vector<std::shared_ptr<ElayGraphics::STexture>>>("BakeChebyshevsTextures");

	auto OctAlbedoTextureConfig = std::make_shared<ElayGraphics::STexture>(); 
	auto OctNormalTextureConfig = std::make_shared<ElayGraphics::STexture>();
	auto OctChebyshevsTextureConfig = std::make_shared<ElayGraphics::STexture>();
	int Level = 2;
	int Width = BakeAlbedoTextures.size() * BakeResolution / Level;
	int Height = BakeResolution * Level;
	OctNormalTextureConfig->InternalFormat = OctAlbedoTextureConfig->InternalFormat = GL_RGBA32F;
	OctNormalTextureConfig->ExternalFormat = OctAlbedoTextureConfig->ExternalFormat = GL_RGBA;
	OctNormalTextureConfig->DataType = OctAlbedoTextureConfig->DataType = GL_FLOAT;
	OctNormalTextureConfig->Type4MinFilter = OctAlbedoTextureConfig->Type4MinFilter = GL_LINEAR;
	OctNormalTextureConfig->Type4MagFilter = OctAlbedoTextureConfig->Type4MagFilter = GL_LINEAR;
	OctNormalTextureConfig->Width = OctAlbedoTextureConfig->Width = Width;
	OctNormalTextureConfig->Height = OctAlbedoTextureConfig->Height = Height;

	OctChebyshevsTextureConfig->InternalFormat = GL_RG32F;
	OctChebyshevsTextureConfig->ExternalFormat = GL_RG;
	OctChebyshevsTextureConfig->DataType = GL_FLOAT;
	OctChebyshevsTextureConfig->Type4MinFilter = GL_LINEAR;
	OctChebyshevsTextureConfig->Type4MagFilter = GL_LINEAR;
	OctChebyshevsTextureConfig->ImageBindUnit = 0;
	OctChebyshevsTextureConfig->Width = Width;
	OctChebyshevsTextureConfig->Height = Height;

	OctAlbedoTextureConfig->ImageBindUnit = 0;
	OctNormalTextureConfig->ImageBindUnit = 1;
	OctChebyshevsTextureConfig->ImageBindUnit = 2;
	genTexture(OctAlbedoTextureConfig);
	genTexture(OctNormalTextureConfig);
	genTexture(OctChebyshevsTextureConfig);
	ElayGraphics::ResourceManager::registerSharedData("OctRadianceTexture", OctAlbedoTextureConfig);
	ElayGraphics::ResourceManager::registerSharedData("OctNormalTexture", OctNormalTextureConfig);
	ElayGraphics::ResourceManager::registerSharedData("OctChebyshevsTexture", OctChebyshevsTextureConfig);

	
	m_pShader = std::make_shared<CShader>("OctahedronPass_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_BakeResolution", BakeResolution);
	m_pShader->setIntUniformValue("u_ImageWidthNum", BakeAlbedoTextures.size()  / Level);
	m_pShader->setIntUniformValue("u_ImageHeightNum", Level);
	for (int i = 0; i < BakeAlbedoTextures.size(); i++)
	{
		m_pShader->setTextureUniformValue("u_RadianceImages[" + std::to_string(i) + "]", BakeAlbedoTextures[i]);
		m_pShader->setTextureUniformValue("u_NormalImage[" + std::to_string(i) + "]", BakeNormalTextures[i]);
		m_pShader->setTextureUniformValue("u_ChebyshevsImage[" + std::to_string(i) + "]", BakeChebyshevsTextures[i]);
	}

	m_pShader->setImageUniformValue(OctAlbedoTextureConfig);
	m_pShader->setImageUniformValue(OctNormalTextureConfig);
	m_pShader->setImageUniformValue(OctChebyshevsTextureConfig);
	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((Width + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((Height + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
}

void COctahedronPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}