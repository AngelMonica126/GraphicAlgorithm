#include "ShadingPass.h"
#include "Shader.h"
#include "Sponza.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>


CShadingPass::CShadingPass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

CShadingPass::~CShadingPass()
{
}

//************************************************************************************
//Function:
void CShadingPass::initV()
{
	//1
	auto Texture1 = std::make_shared<ElayGraphics::STexture>();
	Texture1->Type4WrapR = Texture1->Type4WrapS = Texture1->Type4WrapT = GL_CLAMP_TO_EDGE;
	Texture1->Type4MinFilter = Texture1->Type4MagFilter = GL_LINEAR;
	Texture1->InternalFormat = GL_RGB;
	Texture1->ExternalFormat = GL_RGB;
	Texture1->TextureType = ElayGraphics::STexture::ETextureType::TextureCubeMap;
	std::string path = "../Textures/Skybox/";
	std::vector<std::string> p1;
	for (int i = 0; i < 6; i++)
	{
		int num = i * 3;
		p1.push_back (path + "rendered_" + std::to_string(num) + ".jpg");
	}
	loadCubeTextureFromFile(p1, Texture1);
	//2
	auto Texture2 = std::make_shared<ElayGraphics::STexture>();
	Texture2->Type4WrapR = Texture2->Type4WrapS = Texture2->Type4WrapT = GL_CLAMP_TO_EDGE;
	Texture2->Type4MinFilter = Texture2->Type4MagFilter = GL_LINEAR;
	Texture2->InternalFormat = GL_RGB;
	Texture2->ExternalFormat = GL_RGB;
	Texture2->TextureType = ElayGraphics::STexture::ETextureType::TextureCubeMap;
	std::vector<std::string> p2;
	for (int i = 0; i < 6; i++)
	{
		int num = i * 3 + 1;
		p2.push_back(path + "rendered_" + std::to_string(num) + ".jpg");
	}
	loadCubeTextureFromFile(p2, Texture2);

	//3
	auto Texture3 = std::make_shared<ElayGraphics::STexture>();
	Texture3->Type4WrapR = Texture3->Type4WrapS = Texture3->Type4WrapT = GL_CLAMP_TO_EDGE;
	Texture3->Type4MinFilter = Texture3->Type4MagFilter = GL_LINEAR;
	Texture3->InternalFormat = GL_RGB;
	Texture3->ExternalFormat = GL_RGB;
	Texture3->TextureType = ElayGraphics::STexture::ETextureType::TextureCubeMap;
	std::vector<std::string> p3;
	for (int i = 0; i < 6; i++)
	{
		int num = i * 3 + 2;
		p3.push_back(path + "rendered_" + std::to_string(num) + ".jpg");
	}
	loadCubeTextureFromFile(p3, Texture3);

	auto m_LUTTexture = std::make_shared<ElayGraphics::STexture>();
	loadTextureFromFile("../Textures/BRDFLUT/BRDFLut.dds", m_LUTTexture);
	getCoefs();
	ElayGraphics::Camera::setMainCameraFarPlane(100);
	ElayGraphics::Camera::setMainCameraPos({ -1.57278, 0.244948, 0.367194 });
	ElayGraphics::Camera::setMainCameraFront({ 0.967832, -0.112856, -0.224865 });
	ElayGraphics::Camera::setMainCameraMoveSpeed(0.1);
	m_pShader = std::make_shared<CShader>("Sponza_VS.glsl", "Sponza_FS.glsl");
	m_pSponza = std::dynamic_pointer_cast<CSponza>(ElayGraphics::ResourceManager::getGameObjectByName("Sponza"));
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_BRDFLut", m_LUTTexture);
	m_pShader->setMat4UniformValue("u_ModelMatrix", glm::value_ptr(m_pSponza->getModelMatrix()));
	m_pShader->setTextureUniformValue("u_Texture1", Texture1);
	m_pShader->setTextureUniformValue("u_Texture2", Texture2);
	m_pShader->setTextureUniformValue("u_Texture3", Texture3);
	for (int i = 0; i < m_Coefs.size(); i++)
	{
		m_pShader->setFloatUniformValue("u_Coef[" + std::to_string(i) + "]", m_Coefs[i].x, m_Coefs[i].y, m_Coefs[i].z);
	}
	m_pSponza->initModel(*m_pShader);
}

//************************************************************************************
//Function:
void CShadingPass::getCoefs()
{
	std::string path = "../Textures/Skybox/coefficients.txt";
	std::ifstream ifs(path);
	if (!ifs)
		throw std::runtime_error("open " + path + " failed");
	int i = 0;
	float r, g, b;
	while (ifs >> r >> g >> b)
	{
		m_Coefs.push_back(glm::vec3(r, g, b));
		i++;
	}
}

//************************************************************************************
//Function:
void CShadingPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_pShader->activeShader();
	m_pSponza->updateModel(*m_pShader);
	//drawSphere();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}