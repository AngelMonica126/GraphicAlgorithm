#pragma once
#include "RenderPass.h"
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>
#include "Common.h"


class CGroundPass : public IRenderPass
{
public:
	CGroundPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CGroundPass();

	virtual void initV() override;
	virtual void updateV() override;
private:

};
