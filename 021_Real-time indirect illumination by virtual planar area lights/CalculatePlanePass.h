#pragma once
#include "RenderPass.h"
#include <vector>
#include <GLM/glm.hpp>
#include <GL/glew.h>

struct SPlane
{
	glm::vec4 P0;
	glm::vec4 P1;
	glm::vec4 P2;
	glm::vec4 P3;
	glm::vec4 Normal;
	glm::vec4 SizeAndUV;
	glm::ivec4 Index;//第一位为父节点索引，为-1代表没有父节点，第二位为右孩子索引，第三位为上孩子缩影，-1则没有节点
};

class CCalculatePlane : public IRenderPass
{
public:
	CCalculatePlane(const std::string& vPassName, int vExcutionOrder);
	virtual ~CCalculatePlane();

	virtual void initV() override;
	virtual void updateV() override;

private:
	std::vector<int> m_GlobalGroupSize;
	int ssbo = -1;
};