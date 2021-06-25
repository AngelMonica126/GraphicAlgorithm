#pragma once
#include "RenderPass.h"
#include <GL/glew.h>
class CSponza;

class CAABBDebugPass : public IRenderPass
{
public:
	CAABBDebugPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CAABBDebugPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::shared_ptr<CSponza> m_pSponza;
	glm::ivec3 m_Min;
	glm::ivec3 m_Max;
};