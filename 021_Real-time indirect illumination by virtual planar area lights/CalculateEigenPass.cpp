#include "CalculateEigenPass.h"
#include "Utils.h"
#include "Interface.h"
#include "Shader.h"
#include <random>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "vector"
using namespace std;
CCalculateEigenPass::CCalculateEigenPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CCalculateEigenPass::~CCalculateEigenPass()
{
}

void CCalculateEigenPass::initV()
{
	int block = ElayGraphics::ResourceManager::getSharedDataByName<int>("Block");
	size_t VPLsDataByeteSize = block * block * sizeof(glm::vec4);
	genBuffer(GL_SHADER_STORAGE_BUFFER, VPLsDataByeteSize, nullptr, GL_STATIC_DRAW, 7);
	m_pShader = std::make_shared<CShader>("CalculateEigen_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_Block", block);
	m_pShader->setIntUniformValue("u_MaxLevel", ElayGraphics::ResourceManager::getSharedDataByName<int>("MaxLevel"));
	m_pShader->setTextureUniformValue("u_InputFirstImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("FirstTexture"));
	m_pShader->setTextureUniformValue("u_InputSecondImage", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("SecondTexture"));
	m_pShader->setTextureUniformValue("u_RSMPositionTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("RSMPositionTexture"));
	glm::vec3 lightDir = ElayGraphics::ResourceManager::getSharedDataByName<glm::vec3>("LightDir");	//这是个方向，第四维需要是0，不能为1，否则会出问题
	m_pShader->setFloatUniformValue("u_LightDir", lightDir.x, lightDir.y, lightDir.z);

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((block + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((block + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back(1);
}

void CCalculateEigenPass::updateV()
{

	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
