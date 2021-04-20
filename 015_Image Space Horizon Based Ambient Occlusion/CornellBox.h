#pragma once
#include "GameObject.h"
#include <GLM/glm.hpp>

class CornellBox : public IGameObject
{
public:
	CornellBox(const std::string& vGameObjectName, int vExecutionOrder);

	virtual void initV() override;
	virtual void updateV() override;
};