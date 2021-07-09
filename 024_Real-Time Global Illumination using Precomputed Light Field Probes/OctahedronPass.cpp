#include "OctahedronPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
COctahedronPass::COctahedronPass(const std::string& vPassName, int vExecutionOrder, ElayGraphics::ERenderPassType vtype) : IRenderPass(vPassName, vExecutionOrder, vtype)
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
	m_TextureConfig4Radiances = ElayGraphics::ResourceManager::getSharedDataByName<std::vector<std::shared_ptr<ElayGraphics::STexture>>>("BakeRadianceTextures");
	m_TextureConfig4Normals = ElayGraphics::ResourceManager::getSharedDataByName<std::vector<std::shared_ptr<ElayGraphics::STexture>>>("BakeNormalTextures");
	m_TextureConfig4Chebyshevs = ElayGraphics::ResourceManager::getSharedDataByName<std::vector<std::shared_ptr<ElayGraphics::STexture>>>("BakeChebyshevsTextures");
	
	m_TextureConfig4Radiance = std::make_shared<ElayGraphics::STexture>();
	m_TextureConfig4Normal = std::make_shared<ElayGraphics::STexture>();
	m_TextureConfig4Chebyshev = std::make_shared<ElayGraphics::STexture>();
	m_TextureConfig4Radiance->InternalFormat = m_TextureConfig4Normal->InternalFormat = GL_RGBA16F;
	m_TextureConfig4Radiance->ExternalFormat = m_TextureConfig4Normal->ExternalFormat = GL_RGBA;
	m_TextureConfig4Radiance->DataType = m_TextureConfig4Normal->DataType = GL_FLOAT;
	m_TextureConfig4Radiance->TextureType = m_TextureConfig4Normal->TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	m_TextureConfig4Radiance->Width = m_TextureConfig4Normal->Width = m_BakeResolution;
	m_TextureConfig4Radiance->Height = m_TextureConfig4Normal->Height = m_BakeResolution;
	m_TextureConfig4Radiance->Depth = m_TextureConfig4Normal->Depth = m_TextureConfig4Radiances.size();

	m_TextureConfig4Chebyshev->InternalFormat = GL_RG16F;
	m_TextureConfig4Chebyshev->ExternalFormat = GL_RG;
	m_TextureConfig4Chebyshev->DataType = GL_FLOAT;
	m_TextureConfig4Chebyshev->TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	m_TextureConfig4Chebyshev->Width = m_BakeResolution;
	m_TextureConfig4Chebyshev->Height = m_BakeResolution;
	m_TextureConfig4Chebyshev->Depth = m_TextureConfig4Radiances.size();
	m_TextureConfig4Chebyshev->Type4WrapS = m_TextureConfig4Normal->Type4WrapS = m_TextureConfig4Radiance->Type4WrapS = GL_CLAMP_TO_EDGE;
	m_TextureConfig4Chebyshev->Type4WrapT = m_TextureConfig4Normal->Type4WrapT = m_TextureConfig4Radiance->Type4WrapT = GL_CLAMP_TO_EDGE;
	m_TextureConfig4Chebyshev->Type4WrapR = m_TextureConfig4Normal->Type4WrapR = m_TextureConfig4Radiance->Type4WrapR = GL_CLAMP_TO_EDGE;

	m_TextureConfig4Irradiance = std::make_shared<ElayGraphics::STexture>();
	m_TextureConfig4Irradiance->InternalFormat = GL_RGBA16F;
	m_TextureConfig4Irradiance->ExternalFormat = GL_RGBA;
	m_TextureConfig4Irradiance->DataType = GL_FLOAT;
	m_TextureConfig4Irradiance->TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	m_TextureConfig4Irradiance->Width = m_BakeResolution;
	m_TextureConfig4Irradiance->Height = m_BakeResolution;
	m_TextureConfig4Irradiance->Depth = m_TextureConfig4Radiances.size();


	m_TextureConfig4Radiance->ImageBindUnit = 0;
	m_TextureConfig4Normal->ImageBindUnit = 1;
	m_TextureConfig4Chebyshev->ImageBindUnit = 2;
	m_TextureConfig4Irradiance->ImageBindUnit = 3;

	genTexture(m_TextureConfig4Chebyshev);
	genTexture(m_TextureConfig4Radiance);
	genTexture(m_TextureConfig4Normal);
	genTexture(m_TextureConfig4Irradiance);



	m_pShader = std::make_shared<CShader>("OctahedronPass_VS.glsl", "OctahedronPass_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setImageUniformValue(m_TextureConfig4Radiance);
	m_pShader->setImageUniformValue(m_TextureConfig4Normal);
	m_pShader->setImageUniformValue(m_TextureConfig4Chebyshev);
	m_pShader->setImageUniformValue(m_TextureConfig4Irradiance);

	ElayGraphics::ResourceManager::registerSharedData("OctRadianceTextures", m_TextureConfig4Radiance);
	ElayGraphics::ResourceManager::registerSharedData("OctNormalTextures", m_TextureConfig4Normal);
	ElayGraphics::ResourceManager::registerSharedData("OctChebyshevsTextures", m_TextureConfig4Chebyshev);
	ElayGraphics::ResourceManager::registerSharedData("OctIrradianceTextures", m_TextureConfig4Irradiance);
}
	
//************************************************************************************
//Function:
void COctahedronPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_BakeResolution, m_BakeResolution);
	int Index = 0;
	for (int i = m_Min.x; i < m_Max.x; i++)
		for (int j = m_Min.y; j < m_Max.y; j++)
			for (int k = m_Min.z; k < m_Max.z; k++)
			{
				m_pShader->activeShader();
				m_pShader->setTextureUniformValue("u_BakeRadianceTextures", m_TextureConfig4Radiances[Index]);
				m_pShader->setTextureUniformValue("u_BakeNormalTextures", m_TextureConfig4Normals[Index]);
				m_pShader->setTextureUniformValue("u_BakeChebyshevsTextures", m_TextureConfig4Chebyshevs[Index]);
				m_pShader->setIntUniformValue("u_Index", Index);
				m_pShader->setIntUniformValue("u_BakeResolution", m_BakeResolution);
				drawQuad();
				Index++;
			}
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}