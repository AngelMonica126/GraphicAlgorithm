#include "util.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <stdexcept>
#include <fstream>
#include "cubemap.h"

using namespace std;

Cubemap::Cubemap(std::array<std::string, 6> image_filenames)
{
	for (int i = 0; i < 6; i++)
	{
		cv::Mat img = cv::imread(image_filenames[i]);
		if (!img.data)
			throw std::runtime_error("read image failed: " + image_filenames[i]);
		img.convertTo(images_[i], CV_32FC3, 1.0 / 255.0);
	}
}

Cubemap::Cubemap(std::array<cv::Mat, 6> images)
	:images_(images)
{

}

std::vector<Vertex> Cubemap::getVertices()
{
	int w = Width();
	int h = Height();
	std::vector<Vertex> vertices(w*h * 6);
	for (int k = 0; k < 6; k++)
	{
		cv::Mat img = images_[k];
		for (int j = 0; j < w; j++)
		{
			for (int i = 0; i < h; i++)
			{
				auto c = img.at<cv::Vec3f>(i, j);
				int idx = k * w*h + i * img.cols + j;
				float u = (float)j / (img.cols - 1);
				float v = 1.0f - (float)i / (img.rows - 1);
				Vec3 p = CubeUV2XYZ({ k, u, v });
				vertices[idx] = { p, {c[2], c[1], c[0]} };
			}
		}
	}
	return vertices;
}

cv::Mat Cubemap::GenExpandImage(int maxsize)
{
	int w = Width();
	int h = Height();
	if (w > maxsize)	
		w = maxsize;
	if (h > maxsize)
		h = maxsize;
	
	int xarr[6] = {2*w, 0, w, w, w, 3*w};
	int yarr[6] = {h, h, 0, 2*h, h, h};
	cv::Mat expandimg(3 * h, 4 * w, CV_32FC3);
	
	for (int i = 0; i < 6; i++)
	{
		cv::Rect region(xarr[i], yarr[i], w, h);
		cv::Mat img;
		cv::resize(images_[i], img, cv::Size(w, h));
		img.copyTo(expandimg(region));
	}
	return expandimg;
}

std::vector<Vertex> Cubemap::RandomSample(int n)
{
	vector<Vertex> samples(n);
	for (int i = 0; i < n; i++)
	{
		float x, y, z;		
		do{
			x = NormalRandom();
			y = NormalRandom();
			z = NormalRandom();		
		}while (x == 0 && y == 0 && z == 0);
		Vec3 p(x, y, z);
		Vec3 c = Sample(p);
		samples[i] = { p, c };
	}
	return samples;
}

Vec3 Cubemap::Sample(const Vec3& pos)
{
	CubeUV cubeuv = XYZ2CubeUV(pos);
	
	int j = (int)(cubeuv.u * (Width() - 1));
	int i = (int)((1.f - cubeuv.v)*(Height() - 1));
	auto c = images_[cubeuv.index].at<cv::Vec3f>(i, j);
	return Vec3{ c[2], c[1], c[0] };
}

Vec3 Cubemap::Sample(float theta, float phi)
{
	Vec3 s;
	s.radius = 1;
	s.theta = theta;
	s.phi = phi;
	Vec3 p = Spherical2Cartesian(s);
	return Sample(p);
}
