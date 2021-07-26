#pragma once
#include "RenderPass.h"
#include <GL/glew.h>
#include "DynamicObject.h"
class CSponza;

class GBufferPass : public IRenderPass
{
public:
	GBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~GBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<CShader>m_pDynamicObjectShader;
	std::shared_ptr<CSponza> m_pSponza;
	std::shared_ptr<CDynamicObject> m_pDynamicObject;
	int m_FBO;
};