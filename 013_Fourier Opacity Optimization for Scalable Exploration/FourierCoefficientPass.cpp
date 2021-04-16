#include "FourierCoefficientPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "Dragon.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CFourierCoefficientPass::CFourierCoefficientPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CFourierCoefficientPass::~CFourierCoefficientPass()
{
}

void CFourierCoefficientPass::initV()
{

	auto TextureConfig4CoefficientOne = std::make_shared<ElayGraphics::STexture>();
	auto TextureConfig4CoefficientTwo = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4CoefficientOne->InternalFormat = TextureConfig4CoefficientTwo->InternalFormat = GL_RGBA32F;
	TextureConfig4CoefficientOne->ExternalFormat = TextureConfig4CoefficientTwo->ExternalFormat  = GL_RGBA;
	TextureConfig4CoefficientOne->DataType = TextureConfig4CoefficientTwo->DataType  = GL_FLOAT;
	genTexture(TextureConfig4CoefficientOne);
	genTexture(TextureConfig4CoefficientTwo);

	m_FBO = genFBO({ TextureConfig4CoefficientOne,TextureConfig4CoefficientTwo});

	m_pShader = std::make_shared<CShader>("FourierCoefficient_VS.glsl", "FourierCoefficient_FS.glsl");
	m_pDragon = std::dynamic_pointer_cast<CDragon>(ElayGraphics::ResourceManager::getGameObjectByName("Dragon"));
	m_pShader->activeShader();
	auto NearPlane = ElayGraphics::Camera::getMainCameraNear();
	auto FarPlane = ElayGraphics::Camera::getMainCameraFar();
	m_pShader->setTextureUniformValue("u_OpacityTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("OpacityDepthTexture"));
	m_pShader->setFloatUniformValue("u_NearPlane", static_cast<float>(NearPlane));
	m_pShader->setFloatUniformValue("u_FarPlane", static_cast<float>(FarPlane));
	m_pDragon->initModel(*m_pShader);

	ElayGraphics::ResourceManager::registerSharedData("CoefficientOne", TextureConfig4CoefficientOne);
	ElayGraphics::ResourceManager::registerSharedData("CoefficientTwo", TextureConfig4CoefficientTwo);

}


void CFourierCoefficientPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	m_pShader->activeShader();
	glm::mat4 model = m_pDragon->getModelMatrix();
	for (int i = m_Location.size() - 1; i >= 0; i--)
	{
		glm::mat4 temp = glm::rotate(glm::translate(model, m_Location[i]), 1.57f, glm::vec3(0, 1, 0));
		m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(temp));
		m_pShader->setFloatUniformValue("u_DiffuseColor", m_Color[i].x, m_Color[i].y, m_Color[i].z, m_Color[i].w);
		m_pDragon->updateModel(*m_pShader);
	}
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}