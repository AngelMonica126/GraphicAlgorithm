#include "Sponza.h"
#include "Interface.h"

CSponza::CSponza(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

CSponza::~CSponza()
{
}

void CSponza::initV()
{
	setModel(ElayGraphics::ResourceManager::getOrCreateModel("../Model/sponza/sponza.obj"));
}

void CSponza::updateV()
{
}