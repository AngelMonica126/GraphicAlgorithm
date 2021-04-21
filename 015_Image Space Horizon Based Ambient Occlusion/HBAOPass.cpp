#include "HBAOPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/euler_angles.hpp>
#include <GLM/gtc/random.hpp>
#include <random>

CHBAOPass::CHBAOPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CHBAOPass::~CHBAOPass()
{
}

void CHBAOPass::initV()
{
	m_pShader = std::make_shared<CShader>("HBAOPass_VS.glsl", "HBAOPass_FS.glsl");
	float *HBAONoise = generateNoise();
	auto TextureConfigHBAO = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfigNoise = std::make_shared<ElayGraphics::STexture>();
	TextureConfigHBAO->InternalFormat = GL_RG16;
	TextureConfigHBAO->ExternalFormat = GL_RG;
	TextureConfigHBAO->DataType = GL_FLOAT;

	TextureConfigNoise->InternalFormat = GL_RGBA16F;
	TextureConfigNoise->Width = 4;
	TextureConfigNoise->Height = 4;
	TextureConfigNoise->Type4WrapS = GL_REPEAT;
	TextureConfigNoise->Type4WrapT = GL_REPEAT;
	TextureConfigNoise->Type4MinFilter = GL_NEAREST;
	TextureConfigNoise->Type4MagFilter = GL_NEAREST;

	TextureConfigNoise->ExternalFormat = GL_RGBA;
	TextureConfigNoise->DataType = GL_FLOAT;
	TextureConfigNoise->pDataSet.push_back(HBAONoise);

	genTexture(TextureConfigHBAO);
	genTexture(TextureConfigNoise);

	m_FBO = genFBO({ TextureConfigHBAO});

	ElayGraphics::ResourceManager::registerSharedData("HBAOTexture", TextureConfigHBAO);
	ElayGraphics::ResourceManager::registerSharedData("NoiseTexture", TextureConfigNoise);

	glm::vec2 FocalLen, InvFocalLen, UVToViewA, UVToViewB, LinMAD;
	float fovRad = glm::radians(ElayGraphics::Camera::getMainCameraFov());
	FocalLen[0] = 1.0f / tanf(fovRad * 0.5f) * ((float)ElayGraphics::WINDOW_KEYWORD::getWindowHeight() / (float)ElayGraphics::WINDOW_KEYWORD::getWindowWidth());
	FocalLen[1] = 1.0f / tanf(fovRad * 0.5f);
	InvFocalLen[0] = 1.0f / FocalLen[0];
	InvFocalLen[1] = 1.0f / FocalLen[1];

	UVToViewA[0] = -2.0f * InvFocalLen[0];
	UVToViewA[1] = -2.0f * InvFocalLen[1];
	UVToViewB[0] = 1.0f * InvFocalLen[0];
	UVToViewB[1] = 1.0f * InvFocalLen[1];

	float near = ElayGraphics::Camera::getMainCameraNear(), far = ElayGraphics::Camera::getMainCameraFar();
	LinMAD[0] = (near - far) / (2.0f * near * far);
	LinMAD[1] = (near + far) / (2.0f * near * far);

	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_Near", ElayGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Far", ElayGraphics::Camera::getMainCameraFar());
	m_pShader->setFloatUniformValue("FocalLen", FocalLen.x, FocalLen.y);
	m_pShader->setFloatUniformValue("UVToViewA", UVToViewA.x, UVToViewA.y);
	m_pShader->setFloatUniformValue("UVToViewB", UVToViewB.x, UVToViewB.y);
	m_pShader->setFloatUniformValue("LinMAD", LinMAD.x, LinMAD.y);
	m_pShader->setTextureUniformValue("u_DepthTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("DepthTexture"));
	m_pShader->setTextureUniformValue("u_NoiseTexture", TextureConfigNoise);

}

void CHBAOPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float *CHBAOPass::generateNoise()
{
	float *noise = new float[4 * 4 * 4];
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			glm::vec2 xy = glm::circularRand(1.0f);
			float z = glm::linearRand(0.0f, 1.0f);
			float w = glm::linearRand(0.0f, 1.0f);

			int offset = 4 * (y * 4 + x);
			noise[offset + 0] = xy[0];
			noise[offset + 1] = xy[1];
			noise[offset + 2] = z;
			noise[offset + 3] = w;
		}
	}
	return noise;
}
