#include "FitContinuityPlanePass.h"
#include "Utils.h"
#include "Interface.h"
#include "Shader.h"
#include <random>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "vector"
#include "CalculatePlanePass.h"
using namespace std;
CFitContinuityPlanePass::CFitContinuityPlanePass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CFitContinuityPlanePass::~CFitContinuityPlanePass()
{
}

void CFitContinuityPlanePass::initV()
{
	m_Block = ElayGraphics::ResourceManager::getSharedDataByName<int>("Block");
	size_t VPLsDataByeteSize = m_Block * m_Block * sizeof(SPlane);
	int ssbo = genBuffer(GL_SHADER_STORAGE_BUFFER, VPLsDataByeteSize, nullptr, GL_STATIC_DRAW, 2);
	int count = genBuffer(GL_SHADER_STORAGE_BUFFER, sizeof(int), nullptr, GL_STATIC_DRAW, 3);
	ElayGraphics::ResourceManager::updateSharedDataByName("PlaneSSBO", ssbo);
	ElayGraphics::ResourceManager::updateSharedDataByName("PlaneCountSSBO", count);
	m_pShader = std::make_shared<CShader>("FitContinuityPlane_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_Block", m_Block);
	m_pShader->setIntUniformValue("u_PlaneCount", m_Block * m_Block);
	m_pShader->setFloatUniformValue("u_DistThresh", m_DistThresh);
	m_pShader->setFloatUniformValue("u_NormalThresh", m_NormalThresh);
	m_pShader->setFloatUniformValue("u_Area", ElayGraphics::ResourceManager::getSharedDataByName<float>("LightCameraAreaInWorldSpace") / (m_Block * m_Block));
	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back(1);
	m_GlobalGroupSize.push_back(1);
	m_GlobalGroupSize.push_back(1);
}

void CFitContinuityPlanePass::updateV()
{
	m_pShader->activeShader();
#ifdef _DEBUG
	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_K) == GLFW_PRESS && m_OldKeyKStatus != GLFW_PRESS)
	{
		m_DistThresh += 0.01f;
		m_pShader->setFloatUniformValue("u_DistThresh", m_DistThresh);

		std::cout << m_DistThresh << std::endl;
		m_OldKeyKStatus = GLFW_PRESS;
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_K) == GLFW_RELEASE)
		m_OldKeyKStatus = GLFW_RELEASE;

	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_L) == GLFW_PRESS && m_OldKeyLStatus != GLFW_PRESS)
	{
		m_DistThresh -= 0.01f;
		m_pShader->setFloatUniformValue("u_DistThresh", m_DistThresh);

		std::cout << m_DistThresh  <<std::endl;
		m_OldKeyLStatus = GLFW_PRESS;
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_L) == GLFW_RELEASE)
		m_OldKeyLStatus = GLFW_RELEASE;
	//
	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_H) == GLFW_PRESS && m_OldKeyHStatus != GLFW_PRESS)
	{
		m_NormalThresh += 0.01f;
		m_pShader->setFloatUniformValue("u_NormalThresh", m_NormalThresh);
		std::cout << m_NormalThresh << std::endl;
		m_OldKeyHStatus = GLFW_PRESS;
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_H) == GLFW_RELEASE)
		m_OldKeyHStatus = GLFW_RELEASE;

	if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_J) == GLFW_PRESS && m_OldKeyJStatus != GLFW_PRESS)
	{
		m_NormalThresh -= 0.01f;
		m_pShader->setFloatUniformValue("u_NormalThresh", m_NormalThresh);
		std::cout << m_NormalThresh << std::endl;
		m_OldKeyJStatus = GLFW_PRESS;
	}
	else if (ElayGraphics::InputManager::getKeyStatus(GLFW_KEY_J) == GLFW_RELEASE)
		m_OldKeyJStatus = GLFW_RELEASE;
#endif
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

}
