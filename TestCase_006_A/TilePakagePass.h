#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

class CTilePakagePass : public IRenderPass
{
public:
	CTilePakagePass(const std::string& vPassName, int vExcutionOrder);
	virtual ~CTilePakagePass();

	virtual void initV() override;
	virtual void updateV() override;
	int getHorizontal(std::vector<glm::ivec4> vEdges, int vType);
	int getVertical(std::vector<glm::ivec4> vEdges, int vType);
	int getCross(std::vector<glm::ivec4> vEdges, int vHorizontalType, int vVerticalType);
private:
	std::vector<int> m_GlobalGroupSize;
	int m_Horizontal = 2;
	int m_Vertical = 2;
	std::string m_HorizontalChar[2] = {"B", "Y" };
	std::string m_VerticalChar[2] = {"G", "R" };
	std::vector<glm::ivec4> m_Edges;

	std::shared_ptr<ElayGraphics::STexture> m_InputTextureArray;

};