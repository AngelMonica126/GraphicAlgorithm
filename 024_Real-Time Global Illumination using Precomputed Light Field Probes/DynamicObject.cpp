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
	setModel(ElayGraphics::ResourceManager::getOrCreateModel("../Model/Lucy/Lucy.obj"));//../Model/Dragon/dragon.obj
	setScale({ 0.1,0.1,0.1 });
	//setScale({ 0.005,0.005,0.005 });
	//rotateY(180);
	translate({ -0.2, -2.2, -0.56});
}

void CDynamicObject::updateV()
{
	//setIsFirstFrame(false);
}