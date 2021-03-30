#pragma once

#include <vector>
#include <array>
#include <opencv2/core.hpp>
#include "util.h"

class Harmonics
{
public:
	Harmonics(int degree);
	std::vector<float> Render(Vec3& normal, int width, int height);
	std::array<cv::Mat, 18> RenderCubemap(int width, int height);
private:
	int degree_;

	std::vector<float> Basis(const Vec3& pos);
};