#include "DirectLightPass.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "LightCamera.h"
CDirectLightPass::CDirectLightPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CDirectLightPass::~CDirectLightPass()
{
}

void CDirectLightPass::initV()
{
	auto AlbedoTexture = ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("AlbedoTexture");
	auto NormalTexture = ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("NormalTexture");
	auto PositionTexture = ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("PositionTexture");
	auto LightDepthTexture = ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LightDepthTexture");
	m_pCLightCamera = std::dynamic_pointer_cast<CLightCamera>(ElayGraphics::ResourceManager::getGameObjectByName("LightCamera"));

	auto TextureConfig4DirectIllumination = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4DirectIllumination->InternalFormat = GL_RGBA32F;
	TextureConfig4DirectIllumination->ExternalFormat = GL_RGBA;
	TextureConfig4DirectIllumination->DataType = GL_FLOAT;
	TextureConfig4DirectIllumination->ImageBindUnit = 0;
	genTexture(TextureConfig4DirectIllumination);

	m_pShader = std::make_shared<CShader>("DirectLightPass_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_InputAlbedoTexture", AlbedoTexture);
	m_pShader->setTextureUniformValue("u_InputNormalTexture", NormalTexture);
	m_pShader->setTextureUniformValue("u_InputPositionTexture", PositionTexture);
	m_pShader->setTextureUniformValue("u_LightDepthTexture", LightDepthTexture);
	m_pShader->setImageUniformValue(TextureConfig4DirectIllumination);
	m_pShader->setFloatUniformValue("u_Intensity", ElayGraphics::ResourceManager::getSharedDataByName<float>("LightIntensity"));
	m_LightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", m_LightDir.x, m_LightDir.y, m_LightDir.z);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
	ElayGraphics::ResourceManager::registerSharedData("DirectIlluminationImage", TextureConfig4DirectIllumination);
}

void CDirectLightPass::updateV()
{
	std::vector<glm::mat4> Frustum = m_pCLightCamera->getFrustum();
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_SpiltNum", Frustum.size());
	auto ViewMatrix = ElayGraphics::Camera::getMainCameraViewMatrix();
	for (int i = 0; i < Frustum.size(); i++)
	{
		m_LightVPMatrix = Frustum[i] * ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix");
		m_pShader->setMat4UniformValue("u_LightVPMatrixMulInverseCameraViewMatrix[" + std::to_string(i) +"]", glm::value_ptr(m_LightVPMatrix));

	}
	auto LightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_LightDir", LightDir.x, LightDir.y, LightDir.z);
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}