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
	m_Coefs = vector<glm::vec3>(m_Vertices.size(), glm::vec3());
	Eigen::VectorXf momentsR = Eigen::VectorXf::Zero(12);
	Eigen::VectorXf momentsG = Eigen::VectorXf::Zero(12);
	Eigen::VectorXf momentsB = Eigen::VectorXf::Zero(12);
	int w = m_Images[0].cols;
	int h = m_Images[0].rows;
	for (int k = 0; k < 6; k++)
	{
		cv::Mat img = m_Images[k];
		for (int j = 0; j < w; j++)
		{
			for (int i = 0; i < h; i++)
			{
				float px = i + 0.5;
				float py = j + 0.5;
				float u = 2.0 * (px / w) - 1.0;
				float v = 2.0 * (py / h) - 1.0;
				float d_x = 1.0 / w;
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
				for (int i = 0; i < m_Vertices.size(); i++)
				{
					momentsR[i] += Y[i] * color.r;
					momentsG[i] += Y[i] * color.g;
					momentsB[i] += Y[i] * color.b;
				}
			}
		}
	}
	Eigen::MatrixXf gram = computeGramMatrixSRBF();
	const auto solver = gram.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV);
	Eigen::VectorXf R = solver.solve(momentsR);
	Eigen::VectorXf G = solver.solve(momentsG);
	Eigen::VectorXf B = solver.solve(momentsB);
	for (int i = 0; i < 12; i++)
	{
		m_Coefs[i] = glm::vec3(R[i], G[i], B[i]);
	}
}

glm::vec3 Harmonics::Render(const glm::vec3& pos)
{
	vector<float> Y = Basis(pos);
	glm::vec3 color = glm::vec3(0);
	for (int i = 0; i < m_Vertices.size(); i++)
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

Eigen::MatrixXf Harmonics::computeGramMatrixSRBF()
{
	const glm::u32 sampleCount = 32768;
	float sampleScale = 4 * glm::pi<float>() / float(sampleCount);


	Eigen::MatrixXf gram = Eigen::MatrixXf::Zero(12, 12);

	for (glm::u32 sampleIt = 0; sampleIt < sampleCount; sampleIt += 1) {
		glm::vec2 sample = sampleHammersley(sampleIt, sampleCount);
		glm::vec3 direction = sampleUniformSphere(sample.x, sample.y);

		vector<float> Y = Basis(direction);

		for (glm::u64 lobeAIt = 0; lobeAIt < 12; ++lobeAIt)
		{
			for (glm::u64 lobeBIt = lobeAIt; lobeBIt < 12; ++lobeBIt)
			{
				float delta = Y[lobeAIt] * Y[lobeBIt] * sampleScale;
				gram(lobeAIt, lobeBIt) += delta;

				if (lobeBIt != lobeAIt) {
					gram(lobeBIt, lobeAIt) += delta;
				}
			}
		}
	}

	return gram;
}

vector<float> Harmonics::RenderBasis(const glm::vec3& pos)
{
	vector<float> Y(m_Vertices.size());
	glm::vec3 normal = glm::normalize(pos);
	for (int i = 0; i < m_Vertices.size(); i++)
	{
		float cos = glm::dot(normal, m_Vertices[i]);
		if (cos < 0)
		{
			Y[i] = 0;
			continue;
		}
		cos *= cos;
		Y[i] = 0.7f * 0.5f * cos + 0.3f * 5.0f / 6.0f * cos * cos;
	}
	return Y;
}

vector<float> Harmonics::Basis(const glm::vec3& pos)
{
	vector<float> Y(m_Vertices.size());
	glm::vec3 normal = glm::normalize(pos);
	for (int i = 0; i < m_Vertices.size(); i++)
	{
		float cos = glm::dot(normal, m_Vertices[i]);
		if (cos < 0)
		{
			Y[i] = 0;
			continue;
		}
		cos *= cos;
		Y[i] = 0.7f * 0.5f * cos + 0.3f * 5.0f / 6.0f * cos * cos;
	}
	return Y;
}