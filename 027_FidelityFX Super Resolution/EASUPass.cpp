#include "EASUPass.h"
#include "Interface.h"
#include "Common.h"
#include "Utils.h"
#include "Shader.h"
#include <GLM/gtc/type_ptr.hpp>

CEASUPass::CEASUPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CEASUPass::~CEASUPass()
{
}


void CEASUPass::FsrEasuCon(
	glm::vec4& con0,
	glm::vec4& con1,
	glm::vec4& con2,
	glm::vec4& con3,
	// This the rendered image resolution being upscaled
	float inputViewportInPixelsX,
	float inputViewportInPixelsY,
	// This is the resolution of the resource containing the input image (useful for dynamic resolution)
	float inputSizeInPixelsX,
	float inputSizeInPixelsY,
	// This is the display resolution which the input image gets upscaled to
	float outputSizeInPixelsX,
	float outputSizeInPixelsY) {
	// Output integer position to a pixel position in viewport.
	con0[0] = (inputViewportInPixelsX* 1.0f / (outputSizeInPixelsX));
	con0[1] = (inputViewportInPixelsY* 1.0f / (outputSizeInPixelsY));
	con0[2] = ((0.5)*inputViewportInPixelsX* 1.0f / (outputSizeInPixelsX) - (0.5));
	con0[3] = ((0.5)*inputViewportInPixelsY* 1.0f / (outputSizeInPixelsY) - (0.5));
	// Viewport pixel position to normalized image space.
	// This is used to get upper-left of 'F' tap.
	con1[0] = ( 1.0f / (inputSizeInPixelsX));
	con1[1] = ( 1.0f / (inputSizeInPixelsY));
	// Centers of gather4, first offset from upper-left of 'F'.
	//      +---+---+
	//      |   |   |
	//      +--(0)--+
	//      | b | c |
	//  +---F---+---+---+
	//  | e | f | g | h |
	//  +--(1)--+--(2)--+
	//  | i | j | k | l |
	//  +---+---+---+---+
	//      | n | o |
	//      +--(3)--+
	//      |   |   |
	//      +---+---+
	con1[2] = ((1.0)* 1.0f / (inputSizeInPixelsX));
	con1[3] = ((-1.0)* 1.0f / (inputSizeInPixelsY));
	// These are from (0) instead of 'F'.
	con2[0] = ((-1.0)* 1.0f / (inputSizeInPixelsX));
	con2[1] = ((2.0)* 1.0f / (inputSizeInPixelsY));
	con2[2] = ((1.0)* 1.0f / (inputSizeInPixelsX));
	con2[3] = ((2.0)* 1.0f / (inputSizeInPixelsY));
	con3[0] = ((0.0)* 1.0f / (inputSizeInPixelsX));
	con3[1] = ((4.0)* 1.0f / (inputSizeInPixelsY));
	con3[2] = con3[3] = 0;
}

void CEASUPass::initV()
{
	auto m_InputTexture = std::make_shared<ElayGraphics::STexture>();
	loadTextureFromFile("../Textures/Other/onepiece_low.png", m_InputTexture);

	m_DisplayWidth =  ElayGraphics::WINDOW_KEYWORD::getWindowWidth();
	m_DisplayHeight = ElayGraphics::WINDOW_KEYWORD::getWindowHeight();

	m_RenderWidth = m_InputTexture->Width;
	m_RenderHeight = m_InputTexture->Height;


	auto TextureConfig4EASU = std::make_shared<ElayGraphics::STexture>();
	TextureConfig4EASU->InternalFormat = GL_RGBA32F;
	TextureConfig4EASU->ExternalFormat = GL_RGBA;
	TextureConfig4EASU->DataType = GL_FLOAT;
	TextureConfig4EASU->ImageBindUnit = 0;
	TextureConfig4EASU->Width = m_DisplayWidth;
	TextureConfig4EASU->Height = m_DisplayHeight;
	genTexture(TextureConfig4EASU);
	glm::vec4 con0 = glm::vec4(0);
	glm::vec4 con1 = glm::vec4(0);
	glm::vec4 con2 = glm::vec4(0);
	glm::vec4 con3 = glm::vec4(0);
	FsrEasuCon(con0, con1, con2, con3, m_RenderWidth, m_RenderHeight,
		m_RenderWidth, m_RenderHeight,
		m_DisplayWidth, m_DisplayHeight);
	m_pShader = std::make_shared<CShader>("EASUPass_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_InputTexture", m_InputTexture);
	m_pShader->setFloatUniformValue("con0", con0.x, con0.y, con0.z, con0.w);
	m_pShader->setFloatUniformValue("con1", con1.x, con1.y, con1.z, con1.w);
	m_pShader->setFloatUniformValue("con2", con2.x, con2.y, con2.z, con2.w);
	m_pShader->setFloatUniformValue("con3", con3.x, con3.y, con3.z, con3.w);
	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((m_DisplayWidth + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((m_DisplayHeight + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
	ElayGraphics::ResourceManager::registerSharedData("EASUTexture", TextureConfig4EASU);
	ElayGraphics::ResourceManager::registerSharedData("NormalTexture", m_InputTexture);
	ElayGraphics::ResourceManager::registerSharedData("con", con0);
}

void CEASUPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	
}