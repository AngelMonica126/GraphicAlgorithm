#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class CLightInjectPass : public IRenderPass
{
public:
	CLightInjectPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CLightInjectPass();

	virtual void initV() override;
	virtual void updateV() override;
	void drawVPLVAO();
private:
	glm::ivec3 m_Dimensions;
	int m_FBO = -1;
	int m_VPLVAO = -1;
	std::shared_ptr<ElayGraphics::STexture> TextureConfig4LPVGridR;
	std::shared_ptr<ElayGraphics::STexture> TextureConfig4LPVGridG;
	std::shared_ptr<ElayGraphics::STexture> TextureConfig4LPVGridB;
};