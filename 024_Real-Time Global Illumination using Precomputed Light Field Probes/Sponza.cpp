#include "Sponza.h"
#include "Interface.h"
#include "AABB.h"
CSponza::CSponza(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

CSponza::~CSponza()
{
}

void CSponza::initV()
{
	setModel(ElayGraphics::ResourceManager::getOrCreateModel("../Model/SponzaPBR_dds2tga/SponzaPBR.obj"));
	m_AABB = getAABB();
	//m_Min = glm::vec3(0, -2, 0);
	m_Min = m_AABB->getMin();
	//m_Max = glm::vec3(1, -1, 1);
	m_Max = m_AABB->getMax();
	ElayGraphics::ResourceManager::registerSharedData("MinAABB", m_Min);
	ElayGraphics::ResourceManager::registerSharedData("MaxAABB", m_Max);
}

void CSponza::updateV()
{
}