#include "NormalComputePass.h"
#include "Shader.h"
#include "Utils.h"
#include "Interface.h"

CNormalComputePass::CNormalComputePass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CNormalComputePass::~CNormalComputePass()
{
}

//************************************************************************************
//Function:
void CNormalComputePass::initV()
{
	int block = ElayGraphics::ResourceManager::getSharedDataByName<int>("Block");
	m_pShader = std::make_shared<CShader>("NormalCompute_VS.glsl", "NormalCompute_FS.glsl");
	RSMsize = ElayGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	m_MaxMipLevel = std::log2(RSMsize) - std::log2(block);
	m_FirstTexture = std::make_shared<ElayGraphics::STexture>();
	m_FirstTexture->Width = RSMsize;
	m_FirstTexture->Height = RSMsize;
	m_FirstTexture->InternalFormat = GL_RGBA32F;
	m_FirstTexture->ExternalFormat = GL_RGBA;
	m_FirstTexture->DataType = GL_FLOAT;
	m_FirstTexture->Type4WrapS = m_FirstTexture->Type4WrapT = GL_CLAMP_TO_EDGE;
	m_FirstTexture->isMipmap = GL_TRUE;
	genTexture(m_FirstTexture);

	m_SecondTexture = std::make_shared<ElayGraphics::STexture>();
	m_SecondTexture->Width = RSMsize;
	m_SecondTexture->Height = RSMsize;
	m_SecondTexture->InternalFormat = GL_RGBA32F;
	m_SecondTexture->ExternalFormat = GL_RGBA;
	m_SecondTexture->DataType = GL_FLOAT;
	m_SecondTexture->Type4WrapS = m_SecondTexture->Type4WrapT = GL_CLAMP_TO_EDGE;
	m_SecondTexture->isMipmap = GL_TRUE;
	genTexture(m_SecondTexture);

	m_PingPongFBO= genFBO({ m_FirstTexture,m_SecondTexture });



	ElayGraphics::ResourceManager::registerSharedData("FirstTexture", m_FirstTexture);
	ElayGraphics::ResourceManager::registerSharedData("SecondTexture", m_SecondTexture);
	ElayGraphics::ResourceManager::registerSharedData("MaxLevel", m_MaxMipLevel);

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_PositionImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RSMPositionTexture"));
	m_pShader->setTextureUniformValue("u_NormalImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RSMNormalAndDoubleRoughnessTexture"));
	m_pShader->setTextureUniformValue("u_AlbedoImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RSMAlbedoAndMetallicTexture"));
	m_pShader->setTextureUniformValue("u_GPositionImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("AlbedoTexture"));
	m_pShader->setTextureUniformValue("u_GNormalImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("NormalTexture"));
	m_pShader->setTextureUniformValue("u_GAlbedoImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("PositionTexture"));

	m_pShader->setIntUniformValue("u_MaxLevel", m_MaxMipLevel);
	m_pShader->setTextureUniformValue("u_PlaneCentreTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("PlaneCentreTexture"));
	m_pShader->setFloatUniformValue("u_LightWidth", ElayGraphics::ResourceManager::getSharedDataByName<float>("m_CameraSizeExtent"));
	m_pShader->setIntUniformValue("u_RSMResolution", RSMsize);
	m_pShader->setIntUniformValue("u_Block", block);
}

//************************************************************************************
//Function:
void CNormalComputePass::updateV()
{
	for (int MipLevel = 0; MipLevel <= m_MaxMipLevel; MipLevel+=1)
	{
		int MipWidth = static_cast<int>(RSMsize * std::pow(0.5, MipLevel));
		int MipHeight = MipWidth;
		m_pShader->activeShader();
		m_pShader->setIntUniformValue("u_MipLevel", MipLevel);
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_FirstTexture->TextureID, MipLevel);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_SecondTexture->TextureID, MipLevel);
		glViewport(0, 0, MipWidth, MipHeight);
		m_pShader->setTextureUniformValue("u_InputFirstImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("FirstTexture"));
		m_pShader->setTextureUniformValue("u_InputSecondImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("SecondTexture"));
		glm::vec3 lightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
		m_pShader->setFloatUniformValue("u_LightDir", lightDir.x, lightDir.y, lightDir.z);
		drawQuad();
	}
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
}