#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class CScreenQuadPass : public IRenderPass
{
public:
	CScreenQuadPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CScreenQuadPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_OldKeyMStatusSave = -1;
	GLuint m_Query = -1;
	GLuint64 m_ElapsedTime = -1;
};