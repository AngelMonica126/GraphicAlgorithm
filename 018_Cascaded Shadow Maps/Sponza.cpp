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
	setModel(ElayGraphics::ResourceManager::getOrCreateModel("../Model/lost_empire/lost_empire.obj"));
}

void CSponza::updateV()
{
}