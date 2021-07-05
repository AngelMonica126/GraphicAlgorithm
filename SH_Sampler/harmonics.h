#pragma once

#include <vector>
#include <array>
#include <opencv2/core.hpp>
#include "util.h"

class Harmonics
{
public:
	Harmonics(int vDegree, std::array<std::string, 6> vImageFilenames);
	void Evaluate();
	std::vector<Vec3> getCoefficients()const
	{
		return m_Coefs;
	}
	Vec3 Render(const Vec3& vPos);
	cv::Mat RenderCubemap(int vWidth, int vHeight);
private:
	int m_Degree;
	std::vector<Vec3> m_Coefs;
	std::vector<float> Basis(const Vec3& vPos);
	std::array<cv::Mat, 6> m_Images;
};