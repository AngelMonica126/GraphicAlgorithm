#pragma once
#include "RenderPass.h"

class CScreenQuadPass : public IRenderPass
{
public:
	CScreenQuadPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CScreenQuadPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_VAO = -1;
	int m_HDRTexture = -1;
};