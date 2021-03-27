#include "util.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include "cubemap.h"
#include "harmonics.h"

using namespace std;

std::string CoefficientsString(const std::vector<Vec3>& coefs)
{
	ostringstream oss;
	for (const Vec3& c : coefs)
	{
		oss << c.r << "\t" << c.g << "\t" << c.b << std::endl;
	}
	return oss.str();
}

int main(int argc, char* argv[])
{
	int degree = 3;
	int samplenum = 1000000;

	string dir = "../Textures/Skybox/";
	array<string, 6> faces = { "right", "left", "top", "bottom", "front", "back" };
	array<std::string, 6> img_files;
	string format = "jpg";
	for (int i = 0; i < 6; i++)
		img_files[i] = dir + faces[i] + "." + format;

	bool write_rendered = false;
	degree = 3;
	samplenum = 100000;
	write_rendered = true;

	string outdir = dir;
	if (write_rendered)
	{
		string mkdircmd = "mkdir " + outdir;
		replace(mkdircmd.begin(), mkdircmd.end(), '/', '\\');
		system(mkdircmd.c_str());
	}

	try {

		cout << "reading cubemap ..." << endl;
		Cubemap cubemap(img_files);
		if (write_rendered)
		{
			string expandfile = outdir + "expand." + format;
			cout << "write expand cubemap image: " << expandfile << endl;
			cv::Mat expand = cubemap.GenExpandImage();
			cv::imwrite(expandfile, expand * 255);
		}

		Harmonics harmonics(degree);
		{
			cout << "sampling ..." << endl;
			auto verticies = cubemap.RandomSample(samplenum);
			harmonics.Evaluate(verticies);
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
		return 1;
	}
	return 0;
}