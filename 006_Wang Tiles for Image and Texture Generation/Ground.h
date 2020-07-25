#pragma once
#include "GameObject.h"

class CGround : public IGameObject
{
public:
	CGround(const std::string& vGameObjectName, int vExecutionOrder);

	virtual void initV() override;
	virtual void updateV() override;
};