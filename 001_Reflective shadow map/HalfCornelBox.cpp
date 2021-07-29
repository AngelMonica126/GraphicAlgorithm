#include "HalfCornelBox.h"
#include "Interface.h"

CHalfCornellBox::CHalfCornellBox(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

CHalfCornellBox::~CHalfCornellBox()
{
}

void CHalfCornellBox::initV()
{
	setModel(ElayGraphics::ResourceManager::getOrCreateModel("../Model/sponza/sponza.obj"));//../Model/CornellBox/HalfCornell/HalfCornell.obj
}

void CHalfCornellBox::updateV()
{
}