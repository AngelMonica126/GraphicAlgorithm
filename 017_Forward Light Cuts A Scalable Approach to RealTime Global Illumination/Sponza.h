#pragma once
#include "GameObject.h"

class CSponza : public IGameObject
{
public:
	CSponza(const std::string& vGameObjectName, int vExecutionOrder);
	virtual ~CSponza();

	virtual void initV() override;
	virtual void updateV() override;
	std::vector<glm::vec3> getMTriangle() {
		return m_Triangle;
	}
private:
	std::vector<glm::vec3> m_Triangle;
};