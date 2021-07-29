#include "CSMDebugPass.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "LightCamera.h"
CCSMDebugPass::CCSMDebugPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CCSMDebugPass::~CCSMDebugPass()
{
}

void CCSMDebugPass::initV()
{
	auto PositionTexture = ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("PositionTexture");
	m_pCLightCamera = std::dynamic_pointer_cast<CLightCamera>(ElayGraphics::ResourceManager::getGameObjectByName("LightCamera"));

	auto TextureConfig4CSMDebug = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4CSMDebug->InternalFormat = GL_RGBA32F;
	TextureConfig4CSMDebug->ExternalFormat = GL_RGBA;
	TextureConfig4CSMDebug->DataType = GL_FLOAT;
	TextureConfig4CSMDebug->ImageBindUnit = 0;
	genTexture(TextureConfig4CSMDebug);

	m_pShader = std::make_shared<CShader>("CSMDebugPass_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_InputPositionTexture", PositionTexture);
	m_pShader->setImageUniformValue(TextureConfig4CSMDebug);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowWidth() + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((ElayGraphics::WINDOW_KEYWORD::getWindowHeight() + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
	ElayGraphics::ResourceManager::registerSharedData("CSMDebugImage", TextureConfig4CSMDebug);
}

void CCSMDebugPass::updateV()
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
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}