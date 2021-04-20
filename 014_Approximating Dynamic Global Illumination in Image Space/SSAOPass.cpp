#include "SSAOPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <random>

CSSAOPass::CSSAOPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CSSAOPass::~CSSAOPass()
{
}

void CSSAOPass::initV()
{
	m_pShader = std::make_shared<CShader>("SSAOPass_VS.glsl", "SSAOPass_FS.glsl");
	std::vector<glm::vec3> ssaoKernel = generateKernel();
	std::vector<glm::vec3> ssaoNoise = generateNoise();
	auto TextureConfigSSAO = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfigNoise = std::make_shared<ElayGraphics::STexture>();
	TextureConfigSSAO->InternalFormat = GL_RED;
	TextureConfigSSAO->ExternalFormat = GL_RGB;
	TextureConfigSSAO->DataType = GL_FLOAT;

	TextureConfigNoise->InternalFormat = GL_RGBA32F;
	TextureConfigNoise->Width = 4;
	TextureConfigNoise->Height = 4;
	TextureConfigNoise->ExternalFormat = GL_RGB;
	TextureConfigNoise->DataType = GL_FLOAT;
	TextureConfigNoise->pDataSet.push_back(&ssaoNoise);

	genTexture(TextureConfigSSAO);
	genTexture(TextureConfigNoise);
	m_FBO = genFBO({ TextureConfigSSAO });

	ElayGraphics::ResourceManager::registerSharedData("SSAOTexture", TextureConfigSSAO);
	ElayGraphics::ResourceManager::registerSharedData("NoiseTexture", TextureConfigNoise);

	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_Near", ElayGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Far", ElayGraphics::Camera::getMainCameraFar());
	m_pShader->setFloatUniformValue("u_WindowWidth", ElayGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setFloatUniformValue("u_WindowHeight", ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	m_pShader->setTextureUniformValue("u_PositionTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("PositionTexture"));
	m_pShader->setTextureUniformValue("u_NormalTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("NormalTexture"));
	m_pShader->setTextureUniformValue("u_NoiseTexture", TextureConfigNoise);
	for (int i = 0; i < 64; i++)
		m_pShader->setFloatUniformValue("u_Samples[" + std::to_string(i) + "]", ssaoKernel[i].x, ssaoKernel[i].y, ssaoKernel[i].z);

}

void CSSAOPass::updateV()
{

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
GLfloat CSSAOPass::lerp(GLfloat a, GLfloat b, GLfloat f)
{
	return a + f * (b - a);
}
std::vector<glm::vec3> CSSAOPass::generateKernel()
{
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // 随机浮点数，范围0.0 - 1.0
	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoKernel;
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}
	return ssaoKernel;
}

std::vector<glm::vec3> CSSAOPass::generateNoise()
{
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // 随机浮点数，范围0.0 - 1.0
	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	return ssaoNoise;
}
