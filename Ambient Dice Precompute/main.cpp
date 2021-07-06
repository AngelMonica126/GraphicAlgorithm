#include "util.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include "harmonics.h"

using namespace std;

std::string CoefficientsString(const std::vector<glm::vec3>& vCoefs)
{
	ostringstream oss;
	for (const glm::vec3& c : vCoefs)
	{
		oss << c.r << ",\t" << c.g << ",\t" << c.b << std::endl;
	}
	return oss.str();
}

int main(int argc, char* argv[])
{
	string dir;
	cout << "ÇëÊäÈëÍ¼Æ¬Â·¾¶" << endl;
	cin >> dir;
	if (dir[dir.length() - 1] != '\\')
		dir.push_back('\\');
	array<string, 6> faces = { "right", "left", "top", "bottom", "front", "back" };
	array<std::string, 6> imgFiles;
	string format = "png";
	for (int i = 0; i < 6; i++)
		imgFiles[i] = dir + faces[i] + "." + format;

	string outdir = dir;

	try {
		Harmonics harmonics(imgFiles);
		{
			cout << "sampling ..." << endl;
			harmonics.Evaluate();
			cv::Mat expand = harmonics.RenderCubemap(128, 128);
			string expandfile = outdir + "AmbientDiceExpand." + format;
			cv::imwrite(expandfile, expand * 255);
		}

		cout << "---------- coefficients ----------" << endl;
		auto coefs = harmonics.getCoefficients();
		string coefstr = CoefficientsString(coefs);
		cout << coefstr;
		cout << "----------------------------------" << endl;

		ofstream coeffile(dir + "AmbientDiceExpandCoefficients.txt");
		if (coeffile)
			coeffile << coefstr;
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