#include "LightSource.h"
#include <random>
#include "Utils.h"
#include "Interface.h"
#include "GameObject.h"
#include "AABB.h"

CLightSources::CLightSources(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

CLightSources::~CLightSources()
{
}

void CLightSources::initV()
{
	auto pSponza = ElayGraphics::ResourceManager::getGameObjectByName("Sponza");
	auto pSponzaAABB = pSponza->getAABB();
	auto& MinCorner = pSponzaAABB->getMin();
	auto& MaxCorner = pSponzaAABB->getMax();
	float SponzaVolume = pSponzaAABB->getVolume();
	float LightRadius = 0.5f;
	std::default_random_engine RandomEngine;
	std::uniform_real_distribution<float> UniformFloat01(0.0f, 1.0f);
	std::uniform_real_distribution<float> UniformFloat(0.4f, 0.7f);
	std::uniform_real_distribution<float> UniformFloatX(MinCorner.x, MaxCorner.x);
	std::uniform_real_distribution<float> UniformFloatY(MinCorner.y, MaxCorner.y);
	std::uniform_real_distribution<float> UniformFloatZ(MinCorner.z, MaxCorner.z);

	glm::vec4 ColorAndRadius, Position;
	for (int i = 0; i < m_LightNum; ++i)
	{
		Position = { UniformFloatX(RandomEngine),UniformFloatY(RandomEngine),UniformFloatZ(RandomEngine), 1 };
		hueToRGB(UniformFloat01(RandomEngine)*UniformFloat(RandomEngine), ColorAndRadius);
		ColorAndRadius.w = LightRadius;
		m_LightSources.push_back({ Position,ColorAndRadius });
	}

	ElayGraphics::ResourceManager::registerSharedData("LightSources", m_LightSources.data());
	ElayGraphics::ResourceManager::registerSharedData("LightLightSourcesByteSize", m_LightSources.size() * sizeof(SPointLight));
	ElayGraphics::ResourceManager::registerSharedData("LightNum", m_LightSources.size());
}

void CLightSources::updateV()
{
}