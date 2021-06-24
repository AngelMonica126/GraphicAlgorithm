#include "ScreenSpaceRayTracingPass.h"
#include "Shader.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Sponza.h"
#include "AABB.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CScreenSpaceRayTracingPass::CScreenSpaceRayTracingPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CScreenSpaceRayTracingPass::~CScreenSpaceRayTracingPass()
{
}

void CScreenSpaceRayTracingPass::initV()
{
	m_pShader = std::make_shared<CShader>("GBuffer_VS.glsl", "GBuffer_FS.glsl");
	m_pDynamicObjectShader = std::make_shared<CShader>("ScreenSpaceRayTracing_VS.glsl", "ScreenSpaceRayTracing_FS.glsl");

	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_pDynamicObject = std::dynamic_pointer_cast<CDynamicObject>(ElayGraphics::ResourceManager::getGameObjectByName("DynamicObject"));

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pShader->setFloatUniformValue("u_Near", ElayGraphics::Camera::getMainCameraNear());
	m_pShader->setFloatUniformValue("u_Far", ElayGraphics::Camera::getMainCameraFar());
	m_pShader->setMat4UniformValue("u_TransposeInverseViewModelMatrix", glm::value_ptr(glm::transpose(glm::inverse(ElayGraphics::Camera::getMainCameraViewMatrix() *  m_pSponza->getModelMatrix()))));
	m_pSponza->initModel(*m_pShader);


	//uniform vec3 u_DiffuseColor;
	//uniform sampler2D u_DepthTexture;
	//uniform sampler2D u_AlbedoTexture;
	//uniform float u_Near = 0.1;
	//uniform float u_Far = 1000.0f;
	//uniform vec2 u_DepthSize;
	m_pDynamicObjectShader->activeShader();
	m_pDynamicObjectShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pDynamicObjectShader->setFloatUniformValue("u_Near", ElayGraphics::Camera::getMainCameraNear());
	m_pDynamicObjectShader->setFloatUniformValue("u_Far", ElayGraphics::Camera::getMainCameraFar());
	m_pDynamicObjectShader->setFloatUniformValue("u_DepthSize", 1920.0f,1152.0f);
	m_pDynamicObjectShader->setTextureUniformValue("u_DepthTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("DepthTexture"));
	m_pDynamicObjectShader->setTextureUniformValue("u_AlbedoTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("AlbedoTexture"));
	glm::mat4 screenSpaceProjectionMatrix = glm::mat4(1);
	float width = ElayGraphics::WINDOW_KEYWORD::getWindowWidth();
	float height = ElayGraphics::WINDOW_KEYWORD::getWindowHeight();
	screenSpaceProjectionMatrix[0][0] = width * 0.5f;
	screenSpaceProjectionMatrix[1][1] = height * 0.5f;
	screenSpaceProjectionMatrix[0][3] = width * 0.5f;
	screenSpaceProjectionMatrix[1][3] = height * 0.5f;
	m_pDynamicObjectShader->setMat4UniformValue("_ScreenSpaceProjectionMatrix", glm::value_ptr(screenSpaceProjectionMatrix * ElayGraphics::Camera::getMainCameraProjectionMatrix()));
}

void CScreenSpaceRayTracingPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_pShader->activeShader();
	m_pShader->setMat4UniformValue("u_TransposeInverseViewModelMatrix", glm::value_ptr(glm::transpose(glm::inverse(ElayGraphics::Camera::getMainCameraViewMatrix() *  m_pSponza->getModelMatrix()))));
	m_pSponza->updateModel(*m_pShader);

	m_pDynamicObjectShader->activeShader();
	m_pDynamicObjectShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pDynamicObject->getModelMatrix()));
	drawQuad();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}