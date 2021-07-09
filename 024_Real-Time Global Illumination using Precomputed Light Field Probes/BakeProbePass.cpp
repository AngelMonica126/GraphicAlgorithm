#include "BakeProbePass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CBakeProbePass::CBakeProbePass(const std::string& vPassName, int vExcutionOrder, ElayGraphics::ERenderPassType vtype) : IRenderPass(vPassName, vExcutionOrder, vtype)
{
}

CBakeProbePass::~CBakeProbePass()
{
}

void CBakeProbePass::initV()
{
	m_pShader = std::make_shared<CShader>("BakeProbe_VS.glsl", "BakeProbe_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_MinAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	m_MaxAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");
	//m_Dimensions = MaxAABB - MinAABB;
	for (int i = m_MinAABB.x; i < m_MaxAABB.x; i++)
		for (int j = m_MinAABB.y; j < m_MaxAABB.y; j++)
			for (int k = m_MinAABB.z; k < m_MaxAABB.z; k++)
			{
				auto TextureConfig4Albedo = std::make_shared<ElayGraphics::STexture>();
				auto TextureConfig4Normal = std::make_shared<ElayGraphics::STexture>();
				auto TextureConfig4Chebyshev = std::make_shared<ElayGraphics::STexture>();
				TextureConfig4Albedo->InternalFormat = TextureConfig4Normal->InternalFormat = GL_RGBA32F;
				TextureConfig4Albedo->ExternalFormat = TextureConfig4Normal->ExternalFormat = GL_RGBA;
				TextureConfig4Albedo->DataType = TextureConfig4Normal->DataType = GL_FLOAT;
				TextureConfig4Albedo->TextureType = TextureConfig4Normal->TextureType = ElayGraphics::STexture::ETextureType::TextureCubeMap;
				TextureConfig4Albedo->Width = TextureConfig4Normal->Width = m_BakeResolution;
				TextureConfig4Albedo->Height = TextureConfig4Normal->Height = m_BakeResolution;

				TextureConfig4Chebyshev->InternalFormat = GL_RG32F;
				TextureConfig4Chebyshev->ExternalFormat = GL_RG;
				TextureConfig4Chebyshev->DataType = GL_FLOAT;
				TextureConfig4Chebyshev->TextureType = ElayGraphics::STexture::ETextureType::TextureCubeMap;
				TextureConfig4Chebyshev->Width = m_BakeResolution;
				TextureConfig4Chebyshev->Height = m_BakeResolution;

				genTexture(TextureConfig4Chebyshev);
				genTexture(TextureConfig4Albedo);
				genTexture(TextureConfig4Normal);
				auto TextureConfig4Depth = std::make_shared<ElayGraphics::STexture>();
				TextureConfig4Depth->InternalFormat = GL_RGBA32F;
				TextureConfig4Depth->ExternalFormat = GL_RGBA;
				TextureConfig4Depth->DataType = GL_FLOAT;
				TextureConfig4Depth->TextureType = ElayGraphics::STexture::ETextureType::DepthCubeMap;
				TextureConfig4Depth->TextureAttachmentType = ElayGraphics::STexture::ETextureAttachmentType::CubeDepthTexture;
				TextureConfig4Depth->Width = m_BakeResolution;
				TextureConfig4Depth->Height = m_BakeResolution;
				genTexture(TextureConfig4Depth);
				m_TextureConfig4Albedos.push_back(TextureConfig4Albedo);
				m_TextureConfig4Normals.push_back(TextureConfig4Normal);
				m_TextureConfig4Depths.push_back(TextureConfig4Depth);
				m_TextureConfig4Chebyshevs.push_back(TextureConfig4Chebyshev);
				auto FBO = genFBO({ TextureConfig4Albedo, TextureConfig4Normal,TextureConfig4Chebyshev, TextureConfig4Depth });
				m_FBOs.push_back(FBO);
			}

	ElayGraphics::ResourceManager::registerSharedData("BakeResolution", m_BakeResolution);
	ElayGraphics::ResourceManager::registerSharedData("BakeRadianceTextures", m_TextureConfig4Albedos);
	ElayGraphics::ResourceManager::registerSharedData("BakeNormalTextures", m_TextureConfig4Normals);
	ElayGraphics::ResourceManager::registerSharedData("BakeChebyshevsTextures", m_TextureConfig4Chebyshevs);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pShader->setFloatUniformValue("u_Near", ElayGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Far", ElayGraphics::Camera::getMainCameraFar());
	m_pSponza->initModel(*m_pShader);
}

void CBakeProbePass::updateV()
{
	
	int Index = 0;
	for (int i = m_MinAABB.x; i < m_MaxAABB.x; i++)
		for (int j = m_MinAABB.y; j < m_MaxAABB.y; j++)
			for (int k = m_MinAABB.z; k < m_MaxAABB.z; k++)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[Index]);
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glViewport(0, 0, m_BakeResolution, m_BakeResolution);
				glm::vec3 ViewPos = glm::vec3(i, j, k);
				auto LightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
				for (int i = 0; i < 6; i++)
				{
					glm::vec3 ViewDir = m_BakeDir[i];
					if (abs(ViewDir) == m_LightUpVector)
						ViewDir.z += 0.01f;
					glm::mat4x4 ViewMatrix = glm::lookAt(ViewPos, ViewPos + ViewDir, m_LightUpVector);
					glm::mat4x4 ProjectionMatrix = glm::perspective(m_Fovy, m_Aspect, m_Near, m_Far);

					auto DirLightVPMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix") * ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
					glm::mat4 LightViewMatrixMulInverseCameraViewMatrix = DirLightVPMatrix * glm::inverse(ViewMatrix);

					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureConfig4Albedos[Index]->TextureID, 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureConfig4Normals[Index]->TextureID, 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureConfig4Chebyshevs[Index]->TextureID, 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureConfig4Depths[Index]->TextureID, 0);
					m_pShader->activeShader();
					m_pShader->setMat4UniformValue("u_BakeViewMatrix", glm::value_ptr(ViewMatrix));
					m_pShader->setMat4UniformValue("u_BakeProjectionMatrix", glm::value_ptr(ProjectionMatrix));
					m_pShader->setMat4UniformValue("u_LightVPMatrixMulInverseCameraViewMatrix", glm::value_ptr(LightViewMatrixMulInverseCameraViewMatrix));
					m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
					m_pShader->setFloatUniformValue("u_Intensity", ElayGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));
					m_pSponza->updateModel(*m_pShader);
				}
				Index++;
			}

	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}