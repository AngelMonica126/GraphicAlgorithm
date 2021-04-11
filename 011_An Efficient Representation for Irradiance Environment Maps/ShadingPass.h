#pragma once
#include "RenderPass.h"
#include <memory>
#include <GL/glew.h>

class CSponza;

class CShadingPass : public IRenderPass
{
public:
	CShadingPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CShadingPass();

	virtual void initV() override;
	virtual void updateV() override;
	void getCoefs();
private:
	std::shared_ptr<CSponza> m_pSponza;
	GLint m_FBO;
	std::vector<glm::vec3> m_Coefs;
};