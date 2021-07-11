#pragma once
#include "RenderPass.h"
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include "DynamicObject.h"
class CSponza;

class CRSMBufferPass : public IRenderPass
{
public:
	CRSMBufferPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CRSMBufferPass();

	virtual void initV();
	virtual void updateV();

private:
	std::shared_ptr<CShader>m_pDynamicObjectShader;
	std::shared_ptr<CSponza> m_pSponza;
	std::shared_ptr<CDynamicObject> m_pDynamicObject;
	int m_FBO = -1;
	int m_RSMResolution = 1024;	
};