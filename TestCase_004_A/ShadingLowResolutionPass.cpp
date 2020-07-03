#include "ShadingLowResolutionPass.h"
#include "Interface.h"
#include "Utils.h"
#include "Shader.h"
#include "stack"
CShadingLowResolutionPass::CShadingLowResolutionPass(const std::string& vPassName, int vExcutionOrder, ElayGraphics::ERenderPassType vtype) : IRenderPass(vPassName, vExcutionOrder, vtype)
{
}

CShadingLowResolutionPass::~CShadingLowResolutionPass()
{
}

void CShadingLowResolutionPass::initV()
{
	 pLightSources = CLightSources::get_instance().getPointLights();
	//auto LightNum = ElayGraphics::ResourceManager::getSharedDataByName<size_t>("LightNum");
	LightNum = pLightSources->size();
	 LightSourecesByteSize = LightNum * sizeof(SPointLight);
	int LowWidth = ElayGraphics::ResourceManager::getSharedDataByName<int>("LowWidth");
	int LowHeight = ElayGraphics::ResourceManager::getSharedDataByName<int>("LowHeight");
	blockx = ElayGraphics::ResourceManager::getSharedDataByName<int>("SubBufferNumX");
	blocky = ElayGraphics::ResourceManager::getSharedDataByName<int>("SubBufferNumY");
	int SqrtLightNum = int(sqrt(LightNum));
	int groupX = SqrtLightNum;
	int groupY = SqrtLightNum;
	for (int i = SqrtLightNum; i >= 1; i--)
		if (LightNum % i == 0)
		{
			groupX = i;
			groupY = LightNum / i;
			break;
		}
	ssbo = genBuffer(GL_SHADER_STORAGE_BUFFER, LightSourecesByteSize, pLightSources->data(), GL_STATIC_DRAW, 0);
	m_pShader = std::make_shared<CShader>("ShadingLowResolution_CS.glsl");
	m_pShader->activeShader();
	m_pShader->setIntUniformValue("u_LowWidth", LowWidth);
	m_pShader->setIntUniformValue("u_LowHeight", LowHeight);
	m_pShader->setIntUniformValue("u_BlockX", groupX);
	m_pShader->setIntUniformValue("u_BlockY", groupY);
	m_pShader->setTextureUniformValue("u_AlbedoTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LowAlbedoImage"));
	m_pShader->setTextureUniformValue("u_NormalTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LowNormalImage"));
	m_pShader->setTextureUniformValue("u_PositionTexture", ElayGraphics::ResourceManager::getSharedDataByName<std::shared_ptr<ElayGraphics::STexture>>("LowPositionImage"));

	std::vector<int> LocalGroupSize;
	m_pShader->InquireLocalGroupSize(LocalGroupSize);
	m_GlobalGroupSize.push_back((groupX + LocalGroupSize[0] - 1) / LocalGroupSize[0]);
	m_GlobalGroupSize.push_back((groupY + LocalGroupSize[1] - 1) / LocalGroupSize[1]);
	m_GlobalGroupSize.push_back(1);
}


bool cmp(const SPointLight& a, const SPointLight& b)
{
	return a.Importance.x > b.Importance.x; //从大到小排序
}
void CShadingLowResolutionPass::updateV()
{
	m_pShader->activeShader();
	glDispatchCompute(m_GlobalGroupSize[0], m_GlobalGroupSize[1], m_GlobalGroupSize[2]);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	SPointLight* p = (SPointLight*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
	vector<SPointLight> temp;
	vector<SPointLight> res;
	temp.resize(LightNum);
	for (int i = 0; i < LightNum; i++)
		temp[i] = p[i];
	sort(temp.begin(), temp.end(), cmp);
	int index = 0;
	int size = blockx * blocky;
	for (int i = 0; i < size; i++)
		for (int j = i; j < LightNum; j += size)
			res.push_back(temp[j]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, LightSourecesByteSize, res.data(), GL_DYNAMIC_DRAW);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}