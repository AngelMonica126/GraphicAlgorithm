#pragma once
#include "RenderPass.h"
#include <GL/glew.h>

class CSponza;

class CTriangleCutPass : public IRenderPass
{
public:
	CTriangleCutPass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CTriangleCutPass();

	virtual void initV();
	virtual void updateV();
	float *getSk();
	float *getSkp(float *Sk);
	float *generateNoise();
	void clearAtomic();
private:
	std::shared_ptr<CSponza> m_pSponza;
	std::shared_ptr<ElayGraphics::STexture> m_TriangleInfoTexture;

	int m_FBO;
	int m_TriangleInfoBuffer;
	int m_AtomicSSBO;
	int m_SkSSBO;
	int m_SkpSSBO;
	int m_NoiseSSBO;

	int m_NoizeNum = 64;
	int m_TriangleSizeByALevel = 300000;

	int m_NumS = 5;
	float m_R = 1;
	float m_D = 0.2 * m_R;
	float m_N = 64;
	float m_u = 1.2;
};