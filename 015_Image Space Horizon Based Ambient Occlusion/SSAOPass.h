#pragma once
#include "RenderPass.h"
#include <GL/glew.h>


class CSSAOPass : public IRenderPass
{
public:
	CSSAOPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CSSAOPass();

	virtual void initV();
	virtual void updateV();
	GLfloat lerp(GLfloat a, GLfloat b, GLfloat f);
	std::vector<glm::vec3> generateKernel();
	std::vector<glm::vec3> generateNoise();

private:
	int m_FBO;
};