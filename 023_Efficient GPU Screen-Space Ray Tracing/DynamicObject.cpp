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
	//setModel(ElayGraphics::ResourceManager::getOrCreateModel("../Model/Lucy/Lucy.obj"));//../Model/Dragon/dragon.obj
	rotateX(90);
	scale(glm::vec3(10, 10, 1));
	translate({ 0, -2.19, -0.56});
}

void CDynamicObject::updateV()
{
	//moveLoop({ 0.0f, 1.0f, 1.2f }, 8.0f);
	//rotateLoop({ 0.0f, 2.0f, 0.0f });
	//setIsFirstFrame(false);
}