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
	glm::ivec3 Distance = m_MaxAABB - m_MinAABB;
	int Num = Distance.x * Distance.y * Distance.z;

	{
		m_TextureConfig4Radiance = std::make_shared<ElayGraphics::STexture>();
		m_TextureConfig4Normal = std::make_shared<ElayGraphics::STexture>();
		m_TextureConfig4Chebyshev = std::make_shared<ElayGraphics::STexture>();

		m_TextureConfig4Radiance->InternalFormat = m_TextureConfig4Normal->InternalFormat = GL_RGBA32F;
		m_TextureConfig4Radiance->ExternalFormat = m_TextureConfig4Normal->ExternalFormat = GL_RGBA;
		m_TextureConfig4Radiance->DataType = m_TextureConfig4Normal->DataType = GL_FLOAT;
		m_TextureConfig4Radiance->TextureType = m_TextureConfig4Normal->TextureType = ElayGraphics::STexture::ETextureType::TextureCubeArray;
		m_TextureConfig4Radiance->Width = m_TextureConfig4Normal->Width = m_BakeResolution;
		m_TextureConfig4Radiance->Height = m_TextureConfig4Normal->Height = m_BakeResolution;
		m_TextureConfig4Radiance->Depth = m_TextureConfig4Normal->Depth = Num;

		m_TextureConfig4Chebyshev->InternalFormat = GL_RG32F;
		m_TextureConfig4Chebyshev->ExternalFormat = GL_RG;
		m_TextureConfig4Chebyshev->DataType = GL_FLOAT;
		m_TextureConfig4Chebyshev->TextureType = ElayGraphics::STexture::ETextureType::TextureCubeArray;
		m_TextureConfig4Chebyshev->Width = m_BakeResolution;
		m_TextureConfig4Chebyshev->Height = m_BakeResolution;
		m_TextureConfig4Chebyshev->Depth = Num;

		m_TextureConfig4Radiance->ImageBindUnit = 0;
		m_TextureConfig4Normal->ImageBindUnit = 1;
		m_TextureConfig4Chebyshev->ImageBindUnit = 2;
		genTexture(m_TextureConfig4Chebyshev);
		genTexture(m_TextureConfig4Radiance);
		genTexture(m_TextureConfig4Normal);

	}

	ElayGraphics::ResourceManager::registerSharedData("BakeResolution", m_BakeResolution);
	ElayGraphics::ResourceManager::registerSharedData("BakeAlbedoTextures", m_TextureConfig4Radiance);
	ElayGraphics::ResourceManager::registerSharedData("BakeNormalTextures", m_TextureConfig4Radiance);
	ElayGraphics::ResourceManager::registerSharedData("BakeChebyshevsTextures", m_TextureConfig4Normal);

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
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glViewport(0, 0, m_BakeResolution, m_BakeResolution);
				glm::vec3 LightPos = glm::vec3(i, j, k);
				for (int i = 0; i < 6; i++)
				{
					glm::vec3 LightDir = m_BakeDir[i];
					if (abs(LightDir) == m_LightUpVector)
						LightDir.z += 0.01f;
					glm::mat4x4 LightViewMatrix = glm::lookAt(LightPos, LightPos + LightDir, m_LightUpVector);
					glm::mat4x4 LightProjectionMatrix = glm::perspective(m_Fovy, m_Aspect, m_Near, m_Far);

					auto ViewMatrix = ElayGraphics::Camera::getMainCameraViewMatrix();
					auto DirLightVPMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix") * ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
					glm::mat4 LightViewMatrixMulInverseCameraViewMatrix = DirLightVPMatrix * glm::inverse(ViewMatrix);

					//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_m_TextureConfig4Radiances[Index]->TextureID, 0);
					//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_m_TextureConfig4Normals[Index]->TextureID, 0);
					//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureConfig4Chebyshevs[Index]->TextureID, 0);
					//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureConfig4Depths[Index]->TextureID, 0);
					m_pShader->activeShader();
					m_pShader->setMat4UniformValue("u_BakeViewMatrix", glm::value_ptr(LightViewMatrix));
					m_pShader->setMat4UniformValue("u_BakeProjectionMatrix", glm::value_ptr(LightProjectionMatrix));
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