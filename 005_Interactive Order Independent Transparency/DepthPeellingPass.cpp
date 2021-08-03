#include "DepthPeellingPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
DepthPeellingPass::DepthPeellingPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

DepthPeellingPass::~DepthPeellingPass()
{
}

void DepthPeellingPass::initV()
{
	ElayGraphics::Camera::setMainCameraFarPlane(100);
	ElayGraphics::Camera::setMainCameraMoveSpeed(0.1);

	m_pShader = std::make_shared<CShader>("DepthPeelling_VS.glsl", "DepthPeelling_FS.glsl");
	
	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));
	for (int i = 0; i < 2; i++)
	{
		auto TextureConfig = std::make_shared<ElayGraphics::STexture>();
		TextureConfig->InternalFormat = GL_RGBA32F_ARB;
		TextureConfig->ExternalFormat = GL_RGBA;
		TextureConfig->DataType = GL_FLOAT;
		genTexture(TextureConfig);
		m_TextureConfig4Color.push_back(TextureConfig);

		auto TextureConfig4Depth = std::make_shared<ElayGraphics::STexture>();
		TextureConfig4Depth->InternalFormat = GL_DEPTH_COMPONENT32F;
		TextureConfig4Depth->ExternalFormat = GL_DEPTH_COMPONENT;
		TextureConfig4Depth->DataType = GL_FLOAT;
		TextureConfig4Depth->Type4MinFilter = GL_NEAREST;
		TextureConfig4Depth->Type4MagFilter = GL_NEAREST;
		TextureConfig4Depth->TextureAttachmentType = ElayGraphics::STexture::ETextureAttachmentType::DepthTexture;

		genTexture(TextureConfig4Depth);
		m_TextureConfig4Depth.push_back(TextureConfig4Depth);

		int FBO = genFBO({ m_TextureConfig4Color[i],m_TextureConfig4Depth[i]});
		m_FBO.push_back(FBO);
	}


	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_Width", ElayGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setFloatUniformValue("u_Height", ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	glm::vec3 m_LightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");	
	m_pShader->setFloatUniformValue("u_LightDir", m_LightDir.x, m_LightDir.y, m_LightDir.z);
	glm::vec3 viewPos = ElayGraphics::Camera::getMainCameraPos();	
	m_pShader->setFloatUniformValue("u_ViewPos", viewPos.x, viewPos.y, viewPos.z);
	m_pSponza->initModel(*m_pShader);

	//BlendPass
	

	m_TextureConfig = std::make_shared<ElayGraphics::STexture>();
	m_TextureConfig->InternalFormat = GL_RGBA32F_ARB;
	m_TextureConfig->ExternalFormat = GL_RGBA;
	m_TextureConfig->DataType = GL_FLOAT;
	genTexture(m_TextureConfig);

	auto TextureConfig4Depth = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4Depth->InternalFormat = GL_DEPTH_COMPONENT32F;
	TextureConfig4Depth->ExternalFormat = GL_DEPTH_COMPONENT;
	TextureConfig4Depth->DataType = GL_FLOAT;
	TextureConfig4Depth->Type4MinFilter = GL_NEAREST;
	TextureConfig4Depth->Type4MagFilter = GL_NEAREST;
	TextureConfig4Depth->TextureAttachmentType = ElayGraphics::STexture::ETextureAttachmentType::DepthTexture;
	genTexture(TextureConfig4Depth);
	m_BlendFBO = genFBO({ m_TextureConfig, TextureConfig4Depth});

	m_BlendShader = std::make_shared<CShader>("BlendPass_VS.glsl", "BlendPass_FS.glsl");
	m_BlendShader->activeShader();
	m_BlendShader->setTextureUniformValue("u_BlendTexture", m_TextureConfig4Color[0]);
	m_BlendShader->setTextureUniformValue("u_ColorTexture", m_TextureConfig4Color[0]);
	ElayGraphics::ResourceManager::registerSharedData("ShadingTexture", m_TextureConfig);

}

void DepthPeellingPass::updateV()
{
	for (int i = 0; i < 4; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO[i % 2]);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		m_pShader->activeShader();
		m_pShader->setIntUniformValue("u_Layer", i);
		glm::vec3 viewPos = ElayGraphics::Camera::getMainCameraPos();	//这是个方向，第四维需要是0，不能为1，否则会出问题
		m_pShader->setFloatUniformValue("u_ViewPos", viewPos.x, viewPos.y, viewPos.z);
		if (i != 0)
			m_pShader->setTextureUniformValue("u_DepthTexture", m_TextureConfig4Depth[(i - 1) % 2]);
		m_pSponza->updateModel(*m_pShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_BlendFBO);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		m_BlendShader->activeShader();
		if (i != 0)
		{
			m_BlendShader->setTextureUniformValue("u_BlendTexture", m_TextureConfig);
			m_BlendShader->setTextureUniformValue("u_ColorTexture", m_TextureConfig4Color[(i - 1) % 2]);
		}
		else
		{
			m_BlendShader->setTextureUniformValue("u_BlendTexture", m_TextureConfig4Color[0]);
			m_BlendShader->setTextureUniformValue("u_ColorTexture", m_TextureConfig4Color[0]);
		}
		drawQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


}