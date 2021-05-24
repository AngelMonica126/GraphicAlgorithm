#pragma once
#include "RenderPass.h"

class CScreenQuadPass : public IRenderPass
{
public:
	CScreenQuadPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CScreenQuadPass();

	virtual void initV() override;
	virtual void updateV() override;
};