#include "Dragon.h"
#include "Interface.h"

CDragon::CDragon(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

CDragon::~CDragon()
{
}

void CDragon::initV()
{
	setModel(ElayGraphics::ResourceManager::getOrCreateModel("../Model/Dragon/dragon.obj"));	
}

void CDragon::updateV()
{
}