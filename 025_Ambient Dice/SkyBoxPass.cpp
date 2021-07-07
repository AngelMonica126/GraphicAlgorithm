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
#include "SkyboxPass.h"


CSkyboxPass::CSkyboxPass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

CSkyboxPass::~CSkyboxPass()
{
}

//************************************************************************************
//Function:
void CSkyboxPass::initV()
{
	std::vector<std::string> faces
	{
		"right.png",
		"left.png",
		"top.png",
		"bottom.png",
		"front.png",
		"back.png"
	};
	m_pShader = std::make_shared<CShader>("Skybox_VS.glsl", "Skybox_FS.glsl");
	auto Texture = std::make_shared<ElayGraphics::STexture>();
	Texture->Type4WrapR = Texture->Type4WrapS = Texture->Type4WrapT = GL_CLAMP_TO_EDGE;
	Texture->Type4MinFilter = Texture->Type4MagFilter = GL_LINEAR;
	Texture->InternalFormat = GL_RGBA;
	Texture->ExternalFormat = GL_RGBA;
	Texture->TextureType = ElayGraphics::STexture::ETextureType::TextureCubeMap;
	std::string path = "AmbientDice/";
	std::vector<std::string> p;
	for (int i = 0; i < faces.size(); i++)
	{
		p.push_back((path + faces[i]));
	}
	loadCubeTextureFromFile(p, Texture);
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_Skybox", Texture);
}


//************************************************************************************
//Function:
void CSkyboxPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	m_pShader->activeShader();
	drawCube();
	glDepthFunc(GL_LESS);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}