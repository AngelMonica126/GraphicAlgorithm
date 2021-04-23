#pragma once
#include "GameObject.h"
#include <GLM/glm.hpp>

class CSponza : public IGameObject
{
public:
	CSponza(const std::string& vGameObjectName, int vExecutionOrder);

	virtual void initV() override;
	virtual void updateV() override;
};