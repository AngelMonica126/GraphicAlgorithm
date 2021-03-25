#include "Sponza.h"
#include "Interface.h"

CSponza::CSponza(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

void CSponza::initV()
{
	setModel(ElayGraphics::ResourceManager::getOrCreateModel("../Model/Dragon/dragon.obj"));
}

void CSponza::updateV()
{
}