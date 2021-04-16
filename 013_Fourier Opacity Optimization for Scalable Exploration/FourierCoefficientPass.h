#pragma once
#include "RenderPass.h"

class CDragon;


class CFourierCoefficientPass : public IRenderPass
{
public:
	CFourierCoefficientPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CFourierCoefficientPass();

	virtual void initV();
	virtual void updateV();
private:
	int m_FBO = -1;
	std::shared_ptr<CDragon> m_pDragon;
	std::vector<glm::vec3> m_Location = { glm::vec3(0, -1.7, 3.0),glm::vec3(0, -1.7, 2.0),glm::vec3(0, -1.7, 1.0),glm::vec3(0, -1.7, 0.0),glm::vec3(0, -1.7, -1.0) };
	std::vector<glm::vec4> m_Color = { glm::vec4(1.0,0,0,0.5),glm::vec4(1.0,1,0,0.5) ,glm::vec4(0.0,1,0,0.5) ,glm::vec4(0.0,0,1,0.5) ,glm::vec4(0.0,1,1,0.5) };
};