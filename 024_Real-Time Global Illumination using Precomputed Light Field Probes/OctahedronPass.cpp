#include "OctahedronPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
COctahedronPass::COctahedronPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

COctahedronPass::~COctahedronPass()
{
}

//************************************************************************************
//Function:
void COctahedronPass::initV()
{
	m_BakeResolution = ElayGraphics::ResourceManager::getSharedDataByName<int>("BakeResolution");
	m_Min = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	m_Max = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");
	m_TextureConfig4Albedos = ElayGraphics::ResourceManager::getSharedDataByName<std::vector<std::shared_ptr<ElayGraphics::STexture>>>("BakeAlbedoTextures");
	m_TextureConfig4Normals = ElayGraphics::ResourceManager::getSharedDataByName<std::vector<std::shared_ptr<ElayGraphics::STexture>>>("BakeNormalTextures");
	m_TextureConfig4Chebyshevs = ElayGraphics::ResourceManager::getSharedDataByName<std::vector<std::shared_ptr<ElayGraphics::STexture>>>("BakeChebyshevsTextures");
	
	m_TextureConfig4Albedo = std::make_shared<ElayGraphics::STexture>();
	m_TextureConfig4Normal = std::make_shared<ElayGraphics::STexture>();
	m_TextureConfig4Chebyshev = std::make_shared<ElayGraphics::STexture>();
	m_TextureConfig4Albedo->InternalFormat = m_TextureConfig4Normal->InternalFormat = GL_RGBA32F;
	m_TextureConfig4Albedo->ExternalFormat = m_TextureConfig4Normal->ExternalFormat = GL_RGBA;
	m_TextureConfig4Albedo->DataType = m_TextureConfig4Normal->DataType = GL_FLOAT;
	m_TextureConfig4Albedo->TextureType = m_TextureConfig4Normal->TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	m_TextureConfig4Albedo->Width = m_TextureConfig4Normal->Width = m_BakeResolution;
	m_TextureConfig4Albedo->Height = m_TextureConfig4Normal->Height = m_BakeResolution;
	m_TextureConfig4Albedo->Depth = m_TextureConfig4Normal->Depth = m_TextureConfig4Albedos.size();

	m_TextureConfig4Chebyshev->InternalFormat = GL_RG32F;
	m_TextureConfig4Chebyshev->ExternalFormat = GL_RG;
	m_TextureConfig4Chebyshev->DataType = GL_FLOAT;
	m_TextureConfig4Chebyshev->TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	m_TextureConfig4Chebyshev->Width = m_BakeResolution;
	m_TextureConfig4Chebyshev->Height = m_BakeResolution;
	m_TextureConfig4Chebyshev->Depth = m_TextureConfig4Albedos.size();

	m_TextureConfig4Albedo->ImageBindUnit = 0;
	m_TextureConfig4Normal->ImageBindUnit = 1;
	m_TextureConfig4Chebyshev->ImageBindUnit = 2;
	genTexture(m_TextureConfig4Chebyshev);
	genTexture(m_TextureConfig4Albedo);
	genTexture(m_TextureConfig4Normal);



	m_pShader = std::make_shared<CShader>("OctahedronPass_VS.glsl", "OctahedronPass_FS.glsl");
	m_pShader->activeShader();
}
	
//************************************************************************************
//Function:
void COctahedronPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, m_BakeResolution, m_BakeResolution);
	int Index = 0;
	for (int i = m_Min.x; i < m_Max.x; i++)
		for (int j = m_Min.y; j < m_Max.y; j++)
			for (int k = m_Min.z; k < m_Max.z; k++)
			{
				m_pShader->activeShader();
				m_pShader->setTextureUniformValue("u_BakeAlbedoTextures", m_TextureConfig4Albedos[Index]);
				m_pShader->setTextureUniformValue("u_BakeNormalTextures", m_TextureConfig4Normals[Index]);
				m_pShader->setTextureUniformValue("u_BakeChebyshevsTextures", m_TextureConfig4Chebyshevs[Index]);
				m_pShader->setIntUniformValue("u_Index", Index);
				drawQuad();
				Index++;
			}
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}