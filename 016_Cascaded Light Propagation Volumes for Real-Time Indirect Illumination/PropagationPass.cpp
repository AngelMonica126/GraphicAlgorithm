#include "PropagationPass.h"
#include "Interface.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
CPropagationPass::CPropagationPass(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CPropagationPass::~CPropagationPass()
{
}

void CPropagationPass::drawVPLVAO()
{

	glm::vec3 MinAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	glm::vec3 MaxAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");
	if (m_VPLVAO == -1)
	{
		std::vector<glm::vec3> Vertices;
		for (int i = 0; i < m_Dimensions.x; i++)
			for (int j = 0; j < m_Dimensions.y; j++)
				for (int k = 0; k < m_Dimensions.z; k++)
					Vertices.push_back(glm::vec3(i, j, k));

		m_VPLVAO = createVAO(&Vertices.front(), Vertices.size() * 3 * sizeof(float), {3});
	}
	glBindVertexArray(m_VPLVAO);
	glDrawArrays(GL_POINTS, 0, m_Dimensions.x * m_Dimensions.y * m_Dimensions.z);
	glBindVertexArray(0);
}


//************************************************************************************
//Function:
void CPropagationPass::initV()
{
	int RSMResolution = ElayGraphics::ResourceManager::getSharedDataByName<int>("RSMResolution");
	glm::vec3 MinAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MinAABB");
	glm::vec3 MaxAABB = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("MaxAABB");

	m_Dimensions = MaxAABB - MinAABB;
	m_TextureConfig4LPVGridR.push_back(ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVGridRTexture"));
	m_TextureConfig4LPVGridG.push_back(ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVGridGTexture"));
	m_TextureConfig4LPVGridB.push_back(ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVGridBTexture"));
	for (int i = 1; i <= m_ProgationNum; i++)
	{
		auto TextureConfig4LPVGridR = std::make_shared<ElayGraphics::STexture>();
		auto TextureConfig4LPVGridG = std::make_shared<ElayGraphics::STexture>();
		auto TextureConfig4LPVGridB = std::make_shared<ElayGraphics::STexture>();
		TextureConfig4LPVGridR->ImageBindUnit = 3;
		TextureConfig4LPVGridG->ImageBindUnit = 4;
		TextureConfig4LPVGridB->ImageBindUnit = 5;
		TextureConfig4LPVGridR->TextureType = TextureConfig4LPVGridG->TextureType = TextureConfig4LPVGridB->TextureType = ElayGraphics::STexture::ETextureType::Texture3D;
		TextureConfig4LPVGridR->InternalFormat = TextureConfig4LPVGridG->InternalFormat = TextureConfig4LPVGridB->InternalFormat = GL_RGBA16F;
		TextureConfig4LPVGridR->ExternalFormat = TextureConfig4LPVGridG->ExternalFormat = TextureConfig4LPVGridB->ExternalFormat = GL_RGBA;
		TextureConfig4LPVGridR->DataType = TextureConfig4LPVGridG->DataType = TextureConfig4LPVGridB->DataType = GL_FLOAT;
		TextureConfig4LPVGridR->Width = TextureConfig4LPVGridG->Width = TextureConfig4LPVGridB->Width = m_Dimensions.x;
		TextureConfig4LPVGridR->Height = TextureConfig4LPVGridG->Height = TextureConfig4LPVGridB->Height = m_Dimensions.y;
		TextureConfig4LPVGridR->Depth = TextureConfig4LPVGridG->Depth = TextureConfig4LPVGridB->Depth = m_Dimensions.z;
		TextureConfig4LPVGridR->Type4WrapS = TextureConfig4LPVGridG->Type4WrapS = TextureConfig4LPVGridB->Type4WrapS = GL_CLAMP_TO_EDGE;
		TextureConfig4LPVGridR->Type4WrapT = TextureConfig4LPVGridG->Type4WrapT = TextureConfig4LPVGridB->Type4WrapT = GL_CLAMP_TO_EDGE;

		genTexture(TextureConfig4LPVGridR);
		genTexture(TextureConfig4LPVGridG);
		genTexture(TextureConfig4LPVGridB);

		m_TextureConfig4LPVGridR.push_back(TextureConfig4LPVGridR);
		m_TextureConfig4LPVGridG.push_back(TextureConfig4LPVGridG);
		m_TextureConfig4LPVGridB.push_back(TextureConfig4LPVGridB);
	}
	
	m_TextureConfig4LPVAccumulatorR = std::make_shared<ElayGraphics::STexture>();
	m_TextureConfig4LPVAccumulatorG = std::make_shared<ElayGraphics::STexture>();
	m_TextureConfig4LPVAccumulatorB = std::make_shared<ElayGraphics::STexture>();
	m_TextureConfig4LPVAccumulatorR->ImageBindUnit = 0;
	m_TextureConfig4LPVAccumulatorG->ImageBindUnit = 1;
	m_TextureConfig4LPVAccumulatorB->ImageBindUnit = 2;
	m_TextureConfig4LPVAccumulatorR->TextureType = m_TextureConfig4LPVAccumulatorG->TextureType = m_TextureConfig4LPVAccumulatorB->TextureType = ElayGraphics::STexture::ETextureType::Texture3D;
	m_TextureConfig4LPVAccumulatorR->InternalFormat = m_TextureConfig4LPVAccumulatorG->InternalFormat = m_TextureConfig4LPVAccumulatorB->InternalFormat = GL_RGBA16F;
	m_TextureConfig4LPVAccumulatorR->ExternalFormat = m_TextureConfig4LPVAccumulatorG->ExternalFormat = m_TextureConfig4LPVAccumulatorB->ExternalFormat = GL_RGBA;
	m_TextureConfig4LPVAccumulatorR->DataType = m_TextureConfig4LPVAccumulatorG->DataType = m_TextureConfig4LPVAccumulatorB->DataType = GL_FLOAT;
	m_TextureConfig4LPVAccumulatorR->Width = m_TextureConfig4LPVAccumulatorG->Width = m_TextureConfig4LPVAccumulatorB->Width = m_Dimensions.x;
	m_TextureConfig4LPVAccumulatorR->Height = m_TextureConfig4LPVAccumulatorG->Height = m_TextureConfig4LPVAccumulatorB->Height = m_Dimensions.y;
	m_TextureConfig4LPVAccumulatorR->Depth = m_TextureConfig4LPVAccumulatorG->Depth = m_TextureConfig4LPVAccumulatorB->Depth = m_Dimensions.z;
	m_TextureConfig4LPVAccumulatorR->Type4WrapS = m_TextureConfig4LPVAccumulatorG->Type4WrapS = m_TextureConfig4LPVAccumulatorB->Type4WrapS = GL_CLAMP_TO_EDGE;
	m_TextureConfig4LPVAccumulatorR->Type4WrapT = m_TextureConfig4LPVAccumulatorG->Type4WrapT = m_TextureConfig4LPVAccumulatorB->Type4WrapT = GL_CLAMP_TO_EDGE;

	genTexture(m_TextureConfig4LPVAccumulatorR);
	genTexture(m_TextureConfig4LPVAccumulatorG);
	genTexture(m_TextureConfig4LPVAccumulatorB);




	m_pShader = std::make_shared<CShader>("PropagationPass_VS.glsl", "PropagationPass_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setFloatUniformValue("u_GridDim", m_Dimensions.x, m_Dimensions.y, m_Dimensions.z);
	m_pShader->setIntUniformValue("u_FirstPropStep", false);
	m_pShader->setTextureUniformValue("LPVGridR", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVGridRTexture"));
	m_pShader->setTextureUniformValue("LPVGridG", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVGridGTexture"));
	m_pShader->setTextureUniformValue("LPVGridB", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVGridBTexture"));
	m_pShader->setTextureUniformValue("GeometryVolume", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LPVGVTexture"));
	m_pShader->setImageUniformValue(m_TextureConfig4LPVAccumulatorR);
	m_pShader->setImageUniformValue(m_TextureConfig4LPVAccumulatorG);
	m_pShader->setImageUniformValue(m_TextureConfig4LPVAccumulatorB);

	ElayGraphics::ResourceManager::registerSharedData("LPVAccumulatorRTexture", m_TextureConfig4LPVAccumulatorR);
	ElayGraphics::ResourceManager::registerSharedData("LPVAccumulatorGTexture", m_TextureConfig4LPVAccumulatorG);
	ElayGraphics::ResourceManager::registerSharedData("LPVAccumulatorBTexture", m_TextureConfig4LPVAccumulatorB);
}

//************************************************************************************
//Function:
void CPropagationPass::updateV()
{
	for (int i = 1; i <= m_ProgationNum; i++)
	{
		ClearTexture(m_TextureConfig4LPVGridR[i], GL_TEXTURE_3D);
		ClearTexture(m_TextureConfig4LPVGridG[i], GL_TEXTURE_3D);
		ClearTexture(m_TextureConfig4LPVGridB[i], GL_TEXTURE_3D);
	}
	ClearTexture(m_TextureConfig4LPVAccumulatorR, GL_TEXTURE_3D);
	ClearTexture(m_TextureConfig4LPVAccumulatorG, GL_TEXTURE_3D);
	ClearTexture(m_TextureConfig4LPVAccumulatorB, GL_TEXTURE_3D);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_Dimensions.x, m_Dimensions.y);
	m_pShader->activeShader();
	for (int i = 1; i <= m_ProgationNum; i++)
	{
		if (i == 0)
		{
			m_pShader->setIntUniformValue("u_FirstPropStep", true);
		}
		else
		{
			m_pShader->setIntUniformValue("u_FirstPropStep", false);
		}
		m_pShader->setTextureUniformValue("LPVGridR", m_TextureConfig4LPVGridR[i - 1]);
		m_pShader->setTextureUniformValue("LPVGridG", m_TextureConfig4LPVGridG[i - 1]);
		m_pShader->setTextureUniformValue("LPVGridB", m_TextureConfig4LPVGridB[i - 1]);
		glBindImageTexture(3, m_TextureConfig4LPVGridR[i]->TextureID, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);
		glBindImageTexture(4, m_TextureConfig4LPVGridG[i]->TextureID, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);
		glBindImageTexture(5, m_TextureConfig4LPVGridB[i]->TextureID, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16F);

		drawVPLVAO();
	}
	glViewport(0, 0, ElayGraphics::WINDOW_KEYWORD::getWindowWidth(), ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}