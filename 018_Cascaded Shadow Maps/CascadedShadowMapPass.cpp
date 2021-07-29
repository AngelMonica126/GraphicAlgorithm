#include "CascadedShadowMapPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "LightCamera.h"
#include "AABB.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>


CascadedShadowMapPass::CascadedShadowMapPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CascadedShadowMapPass::~CascadedShadowMapPass()
{
}

void CascadedShadowMapPass::initV()
{
	m_SplitNum = ElayGraphics::ResourceManager::getSharedDataByName<int>("SplitNum");
	m_pShader = std::make_shared<CShader>("CascadedShadowMap_VS.glsl", "CascadedShadowMap_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_pCLightCamera = std::dynamic_pointer_cast<CLightCamera>(ElayGraphics::ResourceManager::getGameObjectByName("LightCamera"));

	auto TextureConfig4Depth = std::make_shared<ElayGraphics::STexture>(); ;
	TextureConfig4Depth->InternalFormat = GL_RGBA16F;
	TextureConfig4Depth->ExternalFormat = GL_RGBA;
	TextureConfig4Depth->DataType = GL_FLOAT;
	TextureConfig4Depth->Width = TextureConfig4Depth->Height = m_RSMResolution;
	TextureConfig4Depth->Depth = ElayGraphics::ResourceManager::getSharedDataByName<int>("SplitNum");
	TextureConfig4Depth->TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	TextureConfig4Depth->BorderColor = { 0,0,0,0 };
	TextureConfig4Depth->ImageBindUnit = 0;
	genTexture(TextureConfig4Depth);

	ElayGraphics::ResourceManager::registerSharedData("LightDepthTexture", TextureConfig4Depth);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pShader->setFloatUniformValue("u_Intensity", ElayGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));
	m_pShader->setImageUniformValue(TextureConfig4Depth);
	m_pSponza->initModel(*m_pShader);
}

void CascadedShadowMapPass::updateV()
{
	std::vector<glm::mat4> Frustum = m_pCLightCamera->getFrustum();
	glm::mat4 LightViewMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	
	for (int i = 0; i < Frustum.size(); i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glViewport(0, 0, m_RSMResolution, m_RSMResolution);
		m_pShader->activeShader();
		m_pShader->setIntUniformValue("u_Index", i);
		m_pShader->setMat4UniformValue("u_LightVPMatrix", glm::value_ptr(Frustum[i] * LightViewMatrix));
		m_pSponza->updateModel(*m_pShader);
		glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}