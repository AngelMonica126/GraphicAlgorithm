#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class CPropagationPass : public IRenderPass
{
public:
	CPropagationPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~CPropagationPass();

	virtual void initV() override;
	virtual void updateV() override;
	void drawVPLVAO();
private:
	glm::ivec3 m_Dimensions;
	int m_FBO = -1;
	int m_VPLVAO = -1;
	int m_ProgationNum = 8;
	std::vector<std::shared_ptr<ElayGraphics::STexture>> m_TextureConfig4LPVGridR;
	std::vector<std::shared_ptr<ElayGraphics::STexture>> m_TextureConfig4LPVGridG;
	std::vector<std::shared_ptr<ElayGraphics::STexture>> m_TextureConfig4LPVGridB;

	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig4LPVAccumulatorR;
	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig4LPVAccumulatorG;
	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig4LPVAccumulatorB;
};