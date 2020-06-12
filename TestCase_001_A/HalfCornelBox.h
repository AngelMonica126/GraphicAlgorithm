#pragma once
#include "GameObject.h"

class CHalfCornellBox : public IGameObject
{
public:
	CHalfCornellBox(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~CHalfCornellBox();

	virtual void initV() override;
	virtual void updateV() override;
};