#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class CDynamicObject;
class CDynamicObjectPass : public IRenderPass
{
public:
	CDynamicObjectPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CDynamicObjectPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_OldKeyMStatusSave = -1;
	int m_FBO;
	std::shared_ptr<CDynamicObject> m_pSponza;
};