#include "GeometryInjectPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
CGeometryInjectPass::CGeometryInjectPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CGeometryInjectPass::~CGeometryInjectPass()
{
}

void CGeometryInjectPass::drawVPLVAO()
{
	int RSMResolution = ElayGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	if (m_VPLVAO == -1)
	{
		float *VPLVertices = new float[RSMResolution * RSMResolution];
		m_VPLVAO = createVAO(VPLVertices, RSMResolution * RSMResolution, { 2 });
	}
	glBindVertexArray(m_VPLVAO);
	glDrawArrays(GL_POINTS, 0, RSMResolution * RSMResolution);
	glBindVertexArray(0);
}


//************************************************************************************
//Function:
void CGeometryInjectPass::initV()
{
	int RSMResolution = ElayGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	glm::vec3 MinAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	glm::vec3 MaxAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");

	m_Dimensions = MaxAABB - MinAABB;
	TextureConfig4LPVGV = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4LPVGV->ImageBindUnit = 0;
	TextureConfig4LPVGV->TextureType = ElayGraphics::STexture::ETextureType::Texture3D;
	TextureConfig4LPVGV->InternalFormat = GL_RGBA16F;
	TextureConfig4LPVGV->ExternalFormat = GL_RGBA;
	TextureConfig4LPVGV->DataType = GL_FLOAT;
	TextureConfig4LPVGV->Width = m_Dimensions.x;
	TextureConfig4LPVGV->Height = m_Dimensions.y;
	TextureConfig4LPVGV->Depth = m_Dimensions.z;
	TextureConfig4LPVGV->Type4WrapS = GL_CLAMP_TO_EDGE;
	TextureConfig4LPVGV->Type4WrapT = GL_CLAMP_TO_EDGE;
	genTexture(TextureConfig4LPVGV);


	m_pShader = std::make_shared<CShader>("GeometryInjectPass_VS.glsl", "GeometryInjectPass_FS.glsl");
	m_pShader->activeShader();
	float test = ElayGraphics::ResourceManager::getSharedDataByName<float>("LightCameraAreaInWorldSpace");
	glm::vec3 lightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");
	m_pShader->setFloatUniformValue("u_CellSize", 1.0);
	m_pShader->setFloatUniformValue("u_LightDir", lightDir.x, lightDir.y, lightDir.z);
	m_pShader->setFloatUniformValue("u_RSMArea", ElayGraphics::ResourceManager::getSharedDataByName<float>("LightCameraAreaInWorldSpace"));
	m_pShader->setMat4UniformValue("u_LightViewMat", glm::value_ptr(ElayGraphics::ResourceManager::getSharedDataByName<glm::mat4>("LightViewMatrix")));
	m_pShader->setFloatUniformValue("u_MinAABB", MinAABB.x, MinAABB.y , MinAABB.z);
	m_pShader->setIntUniformValue("u_RSMResolution", RSMResolution);
	m_pShader->setTextureUniformValue("u_RSMPositionTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RSMPositionTexture"));
	m_pShader->setTextureUniformValue("u_RSMNormalTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RSMNormalAndDoubleRoughnessTexture"));
	m_pShader->setImageUniformValue(TextureConfig4LPVGV);

	ElayGraphics::ResourceManager::registerSharedData("LPVGVTexture", TextureConfig4LPVGV);

}

//************************************************************************************
//Function:
void CGeometryInjectPass::updateV()
{
	ClearTexture(TextureConfig4LPVGV, GL_TEXTURE_3D);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_Dimensions.x, m_Dimensions.y);
	m_pShader->activeShader();
	drawVPLVAO();
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}