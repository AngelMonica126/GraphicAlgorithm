#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

class CMobileFriendlyEASUPass : public IRenderPass
{
public:
	CMobileFriendlyEASUPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CMobileFriendlyEASUPass();

	virtual void initV() override;
	virtual void updateV() override;
	void fsrEasuCon(
		glm::vec4& con0,
		glm::vec4& con1,
		glm::vec4& con2,
		glm::vec4& con3,
		float inputSizeInPixelsX,
		float inputSizeInPixelsY,
		float outputSizeInPixelsX,
		float outputSizeInPixelsY);
private:
	std::vector<int> m_GlobalGroupSize;
	int m_DisplayWidth;
	int m_DisplayHeight;

	int m_RenderWidth;
	int m_RenderHeight;
};