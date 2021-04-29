#include "LightInjectPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
CLightInjectPass::CLightInjectPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CLightInjectPass::~CLightInjectPass()
{
}

void CLightInjectPass::drawVPLVAO()
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
void CLightInjectPass::initV()
{
	int RSMResolution = ElayGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	glm::vec3 MinAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	glm::vec3 MaxAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");

	m_Dimensions = MaxAABB - MinAABB;
	TextureConfig4LPVGridR = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4LPVGridG = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4LPVGridB = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4LPVGridR->ImageBindUnit = 0;
	TextureConfig4LPVGridG->ImageBindUnit = 1;
	TextureConfig4LPVGridB->ImageBindUnit = 2;
	TextureConfig4LPVGridR->TextureType = TextureConfig4LPVGridG->TextureType = TextureConfig4LPVGridB->TextureType = ElayGraphics::STexture::ETextureType::Texture3D;
	TextureConfig4LPVGridR->InternalFormat = TextureConfig4LPVGridG->InternalFormat = TextureConfig4LPVGridB->InternalFormat = GL_RGBA16F;
	TextureConfig4LPVGridR->ExternalFormat = TextureConfig4LPVGridG->ExternalFormat = TextureConfig4LPVGridB->ExternalFormat = GL_RGBA;
	TextureConfig4LPVGridR->DataType = TextureConfig4LPVGridG->DataType = TextureConfig4LPVGridB->DataType  = GL_FLOAT;
	TextureConfig4LPVGridR->Width = TextureConfig4LPVGridG->Width = TextureConfig4LPVGridB->Width = m_Dimensions.x;
	TextureConfig4LPVGridR->Height = TextureConfig4LPVGridG->Height = TextureConfig4LPVGridB->Height = m_Dimensions.y;
	TextureConfig4LPVGridR->Depth = TextureConfig4LPVGridG->Depth = TextureConfig4LPVGridB->Depth = m_Dimensions.z;
	TextureConfig4LPVGridR->Type4WrapS = TextureConfig4LPVGridG->Type4WrapS = TextureConfig4LPVGridB->Type4WrapS = GL_CLAMP_TO_EDGE;
	TextureConfig4LPVGridR->Type4WrapT = TextureConfig4LPVGridG->Type4WrapT = TextureConfig4LPVGridB->Type4WrapT = GL_CLAMP_TO_EDGE;

	genTexture(TextureConfig4LPVGridR);
	genTexture(TextureConfig4LPVGridG);
	genTexture(TextureConfig4LPVGridB);

	m_pShader = std::make_shared<CShader>("LightInjectPass_VS.glsl", "LightInjectPass_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_CellSize", 1.0);
	m_pShader->setFloatUniformValue("u_MinAABB", MinAABB.x, MinAABB.y , MinAABB.z);
	m_pShader->setIntUniformValue("u_RSMResolution", RSMResolution);
	m_pShader->setTextureUniformValue("u_RSMRadiantFluxTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RSMRadiantFluxTexture"));
	m_pShader->setTextureUniformValue("u_RSMPositionTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RSMPositionTexture"));
	m_pShader->setTextureUniformValue("u_RSMNormalTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RSMNormalAndDoubleRoughnessTexture"));
	
	m_pShader->setImageUniformValue(TextureConfig4LPVGridR);
	m_pShader->setImageUniformValue(TextureConfig4LPVGridG);
	m_pShader->setImageUniformValue(TextureConfig4LPVGridB);

	ElayGraphics::ResourceManager::registerSharedData("LPVGridRTexture", TextureConfig4LPVGridR);
	ElayGraphics::ResourceManager::registerSharedData("LPVGridGTexture", TextureConfig4LPVGridG);
	ElayGraphics::ResourceManager::registerSharedData("LPVGridBTexture", TextureConfig4LPVGridB);
}

//************************************************************************************
//Function:
void CLightInjectPass::updateV()
{
	ClearTexture(TextureConfig4LPVGridR, GL_TEXTURE_3D);
	ClearTexture(TextureConfig4LPVGridG, GL_TEXTURE_3D);
	ClearTexture(TextureConfig4LPVGridB, GL_TEXTURE_3D);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_Dimensions.x, m_Dimensions.y);
	m_pShader->activeShader();
	drawVPLVAO();
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}