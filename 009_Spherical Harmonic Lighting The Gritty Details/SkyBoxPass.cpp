#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "SkyboxPass.h"
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
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
	m_pShader = std::make_shared<CShader>("Skybox_VS.glsl", "Skybox_FS.glsl");
	auto Texture = std::make_shared<ElayGraphics::STexture>();
	int width, height, nrChannels;
	std::string path = "../Textures/Skybox/";
	for (int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load((path + faces[i]).c_str(), &width, &height, &nrChannels, 0);
		Texture->pDataSet.push_back(data);
	}
	Texture->Type4WrapR = Texture->Type4WrapS = Texture->Type4WrapT = GL_CLAMP_TO_EDGE;
	Texture->Type4MinFilter = Texture->Type4MagFilter = GL_LINEAR;
	Texture->Width = width;
	Texture->Height = height;
	Texture->InternalFormat = GL_RGB;
	Texture->ExternalFormat = GL_RGB;
	Texture->TextureType = ElayGraphics::STexture::ETextureType::TextureCubeMap;
	genTexture(Texture);

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