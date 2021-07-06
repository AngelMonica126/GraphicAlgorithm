#pragma once

#include <vector>
#include <array>
#include <opencv2/core.hpp>
#include "util.h"

class Harmonics
{
public:
	Harmonics(std::array<std::string, 6> vImageFilenames);
	void Evaluate();
	std::vector<glm::vec3> getCoefficients()const
	{
		return m_Coefs;
	}
	glm::vec3 Render(const glm::vec3& vPos);
	cv::Mat RenderCubemap(int vWidth, int vHeight);
private:
	int m_Degree = 16;
	std::vector<glm::vec3> m_Coefs;
	std::vector<float> Basis(const glm::vec3& vPos);
	std::array<cv::Mat, 6> m_Images;
};