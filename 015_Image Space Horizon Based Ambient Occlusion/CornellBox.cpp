#include "CornellBox.h"
#include "Interface.h"

CornellBox::CornellBox(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

void CornellBox::initV()
{
	setModel(ElayGraphics::ResourceManager::getOrCreateModel("../Model/lost_empire/lost_empire.obj"));
}

void CornellBox::updateV()
{
}