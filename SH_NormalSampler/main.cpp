#include "util.h"
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include "harmonics.h"

using namespace std;

int main(int argc, char* argv[])
{
	int degree = 2;
	string format = "jpg";
	string outdir = "../Textures/Skybox/";
	Harmonics harmonics(degree);
	auto shimgs = harmonics.RenderCubemap(512, 512);
	for (int i = 0; i < 18; i++)
	{
		string outfile = outdir + "rendered_" + to_string(i) + "." + format;
		cout << "write rendered images: " << outfile << endl;
		cv::imwrite(outfile, shimgs[i] * 255);
	}
	return 0;
}