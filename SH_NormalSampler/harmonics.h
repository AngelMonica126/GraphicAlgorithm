#pragma once

#include <vector>
#include <array>
#include <opencv2/core.hpp>
#include "util.h"

class Harmonics
{
public:
	Harmonics(int degree);
	void Evaluate(const std::vector<Vertex>& vertices);
	std::vector<Vec3> getCoefficients()const
	{
		return coefs;
	}
	Vec3 Render(const Vec3& pos);
	std::array<cv::Mat, 6> RenderCubemap(int width, int height);
private:
	int degree_;
	std::vector<Vec3> coefs;

	std::vector<float> Basis(const Vec3& pos);
	std::vector<float> factorial;
};