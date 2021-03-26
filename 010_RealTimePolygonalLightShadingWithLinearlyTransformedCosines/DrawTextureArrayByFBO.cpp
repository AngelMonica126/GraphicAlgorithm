#include "DrawTextureArrayByFBO.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"

CDrawTextureArrayByFBO::CDrawTextureArrayByFBO(const std::string& vPassName, int vExecutionOrder) : IRenderPass(vPassName, vExecutionOrder)
{
}

CDrawTextureArrayByFBO::~CDrawTextureArrayByFBO()
{
}

//************************************************************************************
//Function:
void CDrawTextureArrayByFBO::initV()
{
	auto InputTextureArray = std::make_shared<ElayGraphics::STexture>();
	int Width = 0, Height = 0, Channels = 0;
	GLvoid* pData = nullptr;
	for (int i = 0; i < 8; ++i)
	{
		stbi_set_flip_vertically_on_load(true);
		pData = stbi_load(("Textures/" + std::to_string(i) + ".png").c_str(), &Width, &Height, &Channels, 0);
		InputTextureArray->pDataSet.push_back(pData);
	}
	InputTextureArray->TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	InputTextureArray->InternalFormat = InputTextureArray->ExternalFormat = (Channels == 4) ? GL_RGBA : GL_RGB;
	InputTextureArray->Width = Width;
	InputTextureArray->Height = Height;
	InputTextureArray->Depth = 8;
	genTexture(InputTextureArray);
	for (auto& Item : InputTextureArray->pDataSet)
		stbi_image_free(Item);

	auto FBOTextureArray = std::make_shared<ElayGraphics::STexture>();
	FBOTextureArray->TextureType = ElayGraphics::STexture::ETextureType::Texture2DArray;
	FBOTextureArray->InternalFormat = FBOTextureArray->ExternalFormat = GL_RGB;
	FBOTextureArray->Width = ElayGraphics::WINDOW_KEYWORD::getWindowWidth();
	FBOTextureArray->Height = ElayGraphics::WINDOW_KEYWORD::getWindowHeight();
	FBOTextureArray->Depth = 5;
	FBOTextureArray->Type4WrapS = GL_CLAMP_TO_BORDER;
	FBOTextureArray->Type4WrapT = GL_CLAMP_TO_BORDER;
	FBOTextureArray->BorderColor = { 1,0,0,1 };
	for (int i = 0; i < 2; ++i)
		genTexture(FBOTextureArray);
	
	ElayGraphics::ResourceManager::registerSharedData("FBOTextureArray", InputTextureArray);
}

//************************************************************************************
//Function:
void CDrawTextureArrayByFBO::updateV()
{

}