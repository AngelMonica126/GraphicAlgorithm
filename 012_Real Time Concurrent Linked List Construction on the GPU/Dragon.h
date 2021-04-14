#pragma once
#include "GameObject.h"

class CDragon : public IGameObject
{
public:
	CDragon(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~CDragon();

	virtual void initV() override;
	virtual void updateV() override;
};