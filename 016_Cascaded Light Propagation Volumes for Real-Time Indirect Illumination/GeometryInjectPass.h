#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class CGeometryInjectPass : public IRenderPass
{
public:
	CGeometryInjectPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CGeometryInjectPass();

	virtual void initV() override;
	virtual void updateV() override;
	void drawVPLVAO();
private:
	glm::ivec3 m_Dimensions;
	int m_FBO = -1;
	int m_VPLVAO = -1;
	std::shared_ptr<ElayGraphics::STexture> TextureConfig4LPVGV;
};