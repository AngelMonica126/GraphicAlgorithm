#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

class CEASUPass : public IRenderPass
{
public:
	CEASUPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CEASUPass();

	virtual void initV() override;
	virtual void updateV() override;
	void FsrEasuCon(
		glm::vec4& con0,
		glm::vec4& con1,
		glm::vec4& con2,
		glm::vec4& con3,
		// This is the resolution of the resource containing the input image (useful for dynamic resolution)
		float inputSizeInPixelsX,
		float inputSizeInPixelsY,
		// This is the display resolution which the input image gets upscaled to
		float outputSizeInPixelsX,
		float outputSizeInPixelsY);
private:
	std::vector<int> m_GlobalGroupSize;
	int m_DisplayWidth;
	int m_DisplayHeight;

	int m_RenderWidth;
	int m_RenderHeight;
};