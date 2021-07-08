#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class COctahedronPass : public IRenderPass
{
public:
	COctahedronPass(const std::string& vPassName, int vExecutionOrder);
	virtual ~COctahedronPass();

	virtual void initV() override;
	virtual void updateV() override;

private:
	int m_FBO;
	int m_BakeResolution;
	std::vector<std::shared_ptr<ElayGraphics::STexture>> m_TextureConfig4Albedos;
	std::vector<std::shared_ptr<ElayGraphics::STexture>> m_TextureConfig4Normals;
	std::vector<std::shared_ptr<ElayGraphics::STexture>> m_TextureConfig4Chebyshevs;

	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig4Albedo;
	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig4Normal;
	std::shared_ptr<ElayGraphics::STexture> m_TextureConfig4Chebyshev;
	glm::ivec3 m_Min;
	glm::ivec3 m_Max;
};