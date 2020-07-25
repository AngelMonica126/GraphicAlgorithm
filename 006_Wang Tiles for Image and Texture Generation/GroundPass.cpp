#include "GroundPass.h"
#include "Common.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <memory>
#include "Utils.h"
#include "Shader.h"
#include "Interface.h"
#include "Ground.h"
CGroundPass::CGroundPass(const std::string& vPassName, int vExcutionOrder) :IRenderPass(vPassName, vExcutionOrder)
{
}

CGroundPass::~CGroundPass()
{
}

//************************************************************************************
//Function:
void CGroundPass::initV()
{
	
	
	m_pShader = std::make_shared<CShader>("Ground_VS.glsl", "Ground_FS.glsl");
	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_TileArrayTextures", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("TextureArray"));
	m_pShader->setIntUniformValue("u_TileWidth", ElayGraphics::ResourceManager::getSharedDataByName<int>("TileWidth"));
	m_pShader->setIntUniformValue("u_TileHeight", ElayGraphics::ResourceManager::getSharedDataByName<int>("TileHeight"));
	m_pShader->setIntUniformValue("u_WindowWidth", ElayGraphics::WINDOW_KEYWORD::getWindowWidth());
	m_pShader->setIntUniformValue("u_WindowHeight", ElayGraphics::WINDOW_KEYWORD::getWindowHeight());
}

//************************************************************************************
//Function:
void CGroundPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShader->activeShader();
	
	drawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}