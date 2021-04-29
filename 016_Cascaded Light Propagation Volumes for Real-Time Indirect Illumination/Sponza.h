#pragma once
#include "GameObject.h"

class CSponza : public IGameObject
{
public:
	CSponza(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~CSponza();

	virtual void initV() override;
	virtual void updateV() override;
	const glm::vec3& getMin() const { return m_Min; }
	const glm::vec3& getMax() const { return m_Max; }
private:
	std::shared_ptr<CAABB>m_AABB;
	glm::vec3 m_Min;
	glm::vec3 m_Max;
};