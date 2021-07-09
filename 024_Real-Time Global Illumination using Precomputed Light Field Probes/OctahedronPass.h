#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class COctahedronPass : public IRenderPass
{
public:
	COctahedronPass(const std::string& vPassName, int vExecutionOrder, ElayGraphics::ERenderPassType vtype);
	virtual ~COctahedronPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_FBO;
	int m_BakeResolution;
	std::vector<std::shared_ptr<ElayGraphics::STexture>> m_TextureConfig4Radiances;
	std::vector<std::shared_ptr<ElayGraphics::STexture>> m_TextureConfig4Normals;
	std::vector<std::shared_ptr<ElayGraphics::STexture>> m_TextureConfig4Chebyshevs;

	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig4Radiance;
	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig4Normal;
	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig4Chebyshev;
	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig4Irradiance;
	glm::ivec3 m_Min;
	glm::ivec3 m_Max;
};