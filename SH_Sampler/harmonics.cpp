#include "util.h"
#include <vector>
#include "harmonics.h"
#include "util.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <stdexcept>
#include <fstream>
using namespace std;

Harmonics::Harmonics(std::array<std::string, 6> vImageFilenames)
{
	for (int i = 0; i < 6; i++)
	{
		cv::Mat img = cv::imread(vImageFilenames[i]);
		if (!img.data)
			throw std::runtime_error("read image failed: " + vImageFilenames[i]);
		img.convertTo(m_Images[i], CV_32FC3, 1.0 / 255.0);
	}
}

void Harmonics::Evaluate()
{
	m_Coefs = vector<glm::vec3>(m_Degree, glm::vec3());
	int w = m_Images[0].cols;
	int h = m_Images[0].rows;
	for (int k = 0; k < 6; k++)
	{
		cv::Mat img = m_Images[k];
		for (int j = 0; j < w; j++)
		{
			for (int i = 0; i < h; i++)
			{
				float px = (float)i + 0.5;
				float py = (float)j + 0.5;
				float u = 2.0 * (px / (float)w) - 1.0;
				float v = 2.0 * (py / (float)h) - 1.0;
				float d_x = 1.0 / (float)w;
				float x0 = u - d_x;
				float y0 = v - d_x;
				float x1 = u + d_x;
				float y1 = v + d_x;
				float d_a = surfaceArea(x0, y0) - surfaceArea(x0, y1) - surfaceArea(x1, y0) + surfaceArea(x1, y1);
				u = (float)j / (img.cols - 1);
				v = 1.0f - (float)i / (img.rows - 1);

				glm::vec3 p = CubeUV2XYZ({ k, u, v });
				auto c = img.at<cv::Vec3f>(i, j);
				glm::vec3 color = { c[2] * d_a, c[1] * d_a,c[0] * d_a };
				vector<float> Y = Basis(p);
				for (int i = 0; i < m_Degree; i++)
				{
					m_Coefs[i] = m_Coefs[i] + Y[i] * color;
				}
			}
		}
	}
}

glm::vec3 Harmonics::Render(const glm::vec3& pos)
{
	vector<float> Y = RenderBasis(pos);
	glm::vec3 color;
	for (int i = 0; i < m_Degree; i++)
	{
		color = color + Y[i] * m_Coefs[i];
	}
	return color;
}

cv::Mat Harmonics::RenderCubemap(int width, int height)
{
	std::array<cv::Mat, 6> imgs;
	for (int k = 0; k < 6; k++)
	{
		imgs[k] = cv::Mat(height, width, CV_32FC3);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				float u = (float)j / (width - 1);
				float v = 1.f - (float)i / (height - 1);
				glm::vec3 pos = CubeUV2XYZ({ k, u, v });
				glm::vec3 color = Render(pos);
				imgs[k].at<cv::Vec3f>(i, j) = { color.b, color.g, color.r };
			}
		}
	}

	int xarr[6] = { 2 * width, 0, width, width, width, 3 * width };
	int yarr[6] = { height, height, 0, 2 * height, height, height };
	cv::Mat expandimg(3 * height, 4 * width, CV_32FC3);

	for (int i = 0; i < 6; i++)
	{
		cv::Rect region(xarr[i], yarr[i], width, height);
		cv::Mat temp;
		cv::resize(imgs[i], temp, cv::Size(width, height));
		temp.copyTo(expandimg(region));
	}
	return expandimg;
}

vector<float> Harmonics::RenderBasis(const glm::vec3& pos)
{
	vector<float> Y(m_Degree);
	glm::vec3 normal = glm::normalize(pos);
	float x = normal.x;
	float y = normal.y;
	float z = normal.z;

	if (m_Degree >= 0)
	{
		Y[0] = 1.f / 2.f * sqrt(1.f / PI);
	}
	if (m_Degree >= 1)
	{
		Y[1] = 2.0 / 3.0 *sqrt(3.f / (4.f * PI)) * z;
		Y[2] = 2.0 / 3.0 *sqrt(3.f / (4.f * PI)) * y;
		Y[3] = 2.0 / 3.0 *sqrt(3.f / (4.f * PI)) * x;
	}
	if (m_Degree >= 2)
	{
		Y[4] = 1.0 / 4.0 *1.f / 2.f * sqrt(15.f / PI) * x * z;
		Y[5] = 1.0 / 4.0 *1.f / 2.f * sqrt(15.f / PI) * z * y;
		Y[6] = 1.0 / 4.0 *1.f / 4.f * sqrt(5.f / PI) * (-x * x - z * z + 2 * y*y);
		Y[7] = 1.0 / 4.0 *1.f / 2.f * sqrt(15.f / PI) * y * x;
		Y[8] = 1.0 / 4.0 *1.f / 4.f * sqrt(15.f / PI) * (x * x - z * z);
	}
	return Y;
}

vector<float> Harmonics::Basis(const glm::vec3& pos)
{
	vector<float> Y(m_Degree);
	glm::vec3 normal = glm::normalize(pos);
	float x = normal.x;
	float y = normal.y;
	float z = normal.z;

	if (m_Degree >= 0)
	{
		Y[0] = 1.f / 2.f * sqrt(1.f / PI);
	}
	if (m_Degree >= 1)
	{
		Y[1] = sqrt(3.f / (4.f * PI)) * z;
		Y[2] = sqrt(3.f / (4.f * PI)) * y;
		Y[3] = sqrt(3.f / (4.f * PI)) * x;
	}
	if (m_Degree >= 2)
	{
		Y[4] = 1.f / 2.f * sqrt(15.f / PI) * x * z;
		Y[5] = 1.f / 2.f * sqrt(15.f / PI) * z * y;
		Y[6] = 1.f / 4.f * sqrt(5.f / PI) * (-x * x - z * z + 2 * y*y);
		Y[7] = 1.f / 2.f * sqrt(15.f / PI) * y * x;
		Y[8] = 1.f / 4.f * sqrt(15.f / PI) * (x * x - z * z);
	}
	if (m_Degree >= 3)
	{
		Y[9]  = 1.f / 4.f * sqrt(35.f / (2.f * PI)) * (3 * x * x - z * z) * z;
		Y[10] = 1.f / 2.f * sqrt(105.f / PI) * x * z * y;
		Y[11] = 1.f / 4.f * sqrt(21.f / (2.f * PI)) * z * (4 * y * y - x * x - z * z);
		Y[12] = 1.f / 4.f * sqrt(7.f / PI)*y*(2 * y*y - 3 * x*x - 3 * z * z);
		Y[13] = 1.f / 4.f * sqrt(21.f / (2.f * PI)) * x * (4 * y * y - x * x - z * z);
		Y[14] = 1.f / 4.f * sqrt(105.f / PI) * (x * x - z * z) * y;
		Y[15] = 1.f / 4.f * sqrt(35.f / (2 * PI)) * (x * x - 3 * z * z) * x;
	}
	return Y;
}