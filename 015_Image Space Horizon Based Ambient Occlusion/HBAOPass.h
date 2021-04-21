#pragma once
#include "RenderPass.h"
#include <GL/glew.h>


class CHBAOPass : public IRenderPass
{
public:
	CHBAOPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CHBAOPass();

	virtual void initV();
	virtual void updateV();
	GLfloat lerp(GLfloat a, GLfloat b, GLfloat f);
	float *generateNoise();

private:
	int m_FBO;
};