#include "DynamicObject.h"
#include "Interface.h"
#include "Utils.h"

CDynamicObject::CDynamicObject(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

CDynamicObject::~CDynamicObject()
{
}

void CDynamicObject::initV()
{
	setModel(ElayGraphics::ResourceManager::getOrCreateModel("../Model/teapot/teapot.obj"));
	setScale({ 0.005,0.005,0.005 });
	translate({ -0.2, -1.2, -0.56});
}

void CDynamicObject::updateV()
{
}