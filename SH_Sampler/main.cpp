#include "util.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include "harmonics.h"

using namespace std;

std::string CoefficientsString(const std::vector<Vec3>& vCoefs)
{
	ostringstream oss;
	for (const Vec3& c : vCoefs)
	{
		oss << c.r << "\t" << c.g << "\t" << c.b << std::endl;
	}
	return oss.str();
}

int main(int argc, char* argv[])
{
	int degree = 2;

	string dir = "C:/Users/veerzeng/Pictures/LightProbe/";
	array<string, 6> faces = { "right", "left", "top", "bottom", "front", "back" };
	array<std::string, 6> imgFiles;
	string format = "png";
	for (int i = 0; i < 6; i++)
		imgFiles[i] = dir + faces[i] + "." + format;

	string outdir = dir;

	try {
		Harmonics harmonics(degree, imgFiles);
		{
			cout << "sampling ..." << endl;
			harmonics.Evaluate();
			cv::Mat expand = harmonics.RenderCubemap(128, 128);
			string expandfile = outdir + "expand." + format;
			cv::imwrite(expandfile, expand * 255);
		}

		cout << "---------- coefficients ----------" << endl;
		auto coefs = harmonics.getCoefficients();
		string coefstr = CoefficientsString(coefs);
		cout << coefstr;
		cout << "----------------------------------" << endl;

		ofstream coeffile(dir + "coefficients.txt");
		if (coeffile)
		{
			coeffile << coefstr;
			cout << "written " << dir + "coefficients.txt" << endl;
		}
		else
			cout << "write coefficients.txt failed" << endl;
	}
	catch (std::exception e)
	{
		cout << "***** AN ERROR OCCURRED *****" << endl;
		cout << e.what() << endl;
		system("pause");
		return 1;
	}
	system("pause");
	return 0;
}