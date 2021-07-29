#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include "DynamicObject.h"
class CSponza;

class CMSMBufferPass : public IRenderPass
{
public:
	CMSMBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CMSMBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<CShader>m_pDynamicObjectShader;
	std::shared_ptr<CSponza> m_pSponza;
	std::shared_ptr<CDynamicObject> m_pDynamicObject;
	int m_FBO = -1;
	int m_RSMResolution = 1024;	
};