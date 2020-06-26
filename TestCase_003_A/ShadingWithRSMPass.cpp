#include "ShadingWithRSMPass.h"
#include "Common.h"
#include "Interface.h"
#include "Utils.h"
#include "Shader.h"
#include <random>
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

CShadingWithRSMPass::CShadingWithRSMPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CShadingWithRSMPass::~CShadingWithRSMPass()
{
}

void CShadingWithRSMPass::initV()
{
	auto TextureConfig = std::make_shared<ElayGraphics::STexture>();
	TextureConfig->InternalFormat = GL_RGBA32F;
	TextureConfig->ExternalFormat = GL_RGBA;
	TextureConfig->DataType = GL_FLOAT;
	TextureConfig->Type4MinFilter = GL_LINEAR;
	TextureConfig->Type4MagFilter = GL_LINEAR;
	TextureConfig->ImageBindUnit = 0;
	genTexture(TextureConfig);
	ElayGraphics::ResourceManager::registerSharedData("ShadingTexture", TextureConfig);

	m_LightViewMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_LightProjectionMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix");
	m_LightVPMatrix = m_LightProjectionMatrix * m_LightViewMatrix;

	m_pShader = std::make_shared<CShader>("ShadingWithRSM_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_AlbedoTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("AlbedoTexture"));
	m_pShader->setTextureUniformValue("u_NormalTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("NormalTexture"));
	m_pShader->setTextureUniformValue("u_PositionTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("PositionTexture"));
	m_pShader->setTextureUniformValue("u_LightDepthTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LightDepthTexture"));
	int RSMSize = ElayGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	m_pShader->setIntUniformValue("u_RSMSize", RSMSize);
	m_pShader->setIntUniformValue("u_VPLsCount", RSMSize * RSMSize);
	m_pShader->setIntUniformValue("u_EnableIndirectIllumination", m_EnableIndirectIllumination);
	m_LightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");	//这是个方向，第四维需要是0，不能为1，否则会出问题

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
}

void CShadingWithRSMPass::updateV()
{
	m_pShader->activeShader();
	auto ViewMatrix = ElayGraphics::Camera::getMainCameraViewMatrix();
#ifdef _DEBUG
	m_LightVPMatrix = ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightProjectionMatrix") * ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
	m_LightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir"), 0.0f;
#endif
	glm::mat4 LightViewMatrixMulInverseCameraViewMatrix = m_LightVPMatrix * glm::inverse(ViewMatrix);
	m_pShader->setMat4UniformValue("u_LightVPMatrixMulInverseCameraViewMatrix", glm::value_ptr(LightViewMatrixMulInverseCameraViewMatrix));
	m_pShader->setFloatUniformValue("u_LightDir", m_LightDir.x, m_LightDir.y, m_LightDir.z);
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_I) == GLFW_PRESS && m_OldKeyIStatus != GLFW_PRESS)
	{
		m_OldKeyIStatus = GLFW_PRESS;
		m_EnableIndirectIllumination = !m_EnableIndirectIllumination;
		m_pShader->setIntUniformValue("u_EnableIndirectIllumination", m_EnableIndirectIllumination);
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_I) == GLFW_RELEASE)
		m_OldKeyIStatus = GLFW_RELEASE;
}