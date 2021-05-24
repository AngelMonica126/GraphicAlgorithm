#pragma once
#include "RenderPass.h"
#include <memory>
#include <GL/glew.h>

class EncodePass : public IRenderPass
{
public:
	EncodePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~EncodePass();

	virtual void initV() override;
	virtual void updateV() override;
private:
	GLint m_FBO;
};