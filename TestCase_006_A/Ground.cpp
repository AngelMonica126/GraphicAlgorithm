#include "Ground.h"
#include "Interface.h"

CGround::CGround(const std::string& vGameObjectName, int vExecutionOrder) : IGameObject(vGameObjectName, vExecutionOrder)
{
}

void CGround::initV()
{
	rotateX(90);
	scale(glm::vec3(10, 10, 1));
}

void CGround::updateV()
{
}