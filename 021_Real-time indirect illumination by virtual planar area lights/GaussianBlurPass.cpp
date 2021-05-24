#include "GaussianBlurPass.h"
#include "Shader.h"
#include "Utils.h"
#include "Interface.h"

CGaussianBlurPass::CGaussianBlurPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CGaussianBlurPass::~CGaussianBlurPass()
{
}

//************************************************************************************
//Function:
void CGaussianBlurPass::initV()
{
	m_pShader = std::make_shared<CShader>("GaussianBlur_VS.glsl", "GaussianBlur_FS.glsl");
	m_InputTexture = std::make_shared<ElayGraphics::STexture>();
	RSMsize = ElayGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	for (int i = 0; i < 2; ++i)
	{
		auto Texture2D = std::make_shared<ElayGraphics::STexture>();
		Texture2D->Width = RSMsize;
		Texture2D->Height = RSMsize;
		Texture2D->InternalFormat = GL_RGBA16F;
		Texture2D->ExternalFormat = GL_RGBA;
		Texture2D->DataType = GL_FLOAT;
		Texture2D->Type4WrapS = Texture2D->Type4WrapT = GL_CLAMP_TO_EDGE;
		Texture2D->isMipmap = GL_TRUE;
		genTexture(Texture2D);
		m_PingPongTexture.push_back(Texture2D);
		m_PingPongFBO[i] = genFBO({Texture2D});
	}
	ElayGraphics::ResourceManager::registerSharedData("BluredTexture", m_PingPongTexture[0]);
	ElayGraphics::ResourceManager::registerSharedData("MaxMipLevel", m_MaxMipLevel);
	m_InputTexture = ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("WeightAlbedoImage");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_InputImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("WeightAlbedoImage"));
	
}

//************************************************************************************
//Function:
void CGaussianBlurPass::updateV()
{
	for (int MipLevel = 0; MipLevel < m_MaxMipLevel; MipLevel+=1)
	{
		int MipWidth = static_cast<int>(RSMsize * std::pow(0.5, MipLevel));
		int MipHeight = MipWidth;
		bool IsHorizontal = true, IsFirstIteration = true;
		int BlurAmount = 10;
		m_pShader->activeShader();
		m_pShader->setIntUniformValue("u_MipLevel", MipLevel);
		for (int i = 0; i < BlurAmount; ++i)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFBO[IsHorizontal]);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_PingPongTexture[IsHorizontal]->TextureID, MipLevel);
			glViewport(0, 0, MipWidth, MipHeight);
			glClearColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_pShader->setIntUniformValue("u_IsHorizontal", IsHorizontal);
			m_pShader->setTextureUniformValue("u_InputImage", IsFirstIteration ? m_InputTexture : m_PingPongTexture[!IsHorizontal]);
			drawQuad();
			IsHorizontal = !IsHorizontal;
			if (IsFirstIteration) 
				IsFirstIteration = false;
		}
	}
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
}