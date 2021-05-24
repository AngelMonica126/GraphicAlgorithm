#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>


class CCalculateWeight : public IRenderPass
{
public:
	CCalculateWeight(const std::string& vPassName, int vExcutionOrder);
	virtual ~CCalculateWeight();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig4AlbedoAndMetallic;
	int ssbo = -1;
};