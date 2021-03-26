#include <gli.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <glm/glm.hpp>
using namespace std;

const float PI = 3.14159265358979323846264338327950288;

float RadicalInverse_VdC(unsigned int bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10;
}

glm::vec2 Hammersley(unsigned int i, unsigned int N)
{
	return glm::vec2(float(i) / float(N), RadicalInverse_VdC(i));
}

glm::vec3 ImportanceSampleGGX(glm::vec2 Xi, float roughness, glm::vec3 N)
{
	float a = roughness * roughness;

	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	// from spherical coordinates to cartesian coordinates
	glm::vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;

	// from tangent-space vector to world-space sample vector
	glm::vec3 up = abs(N.z) < 0.999 ? glm::vec3(0.0, 0.0, 1.0) : glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 tangent = normalize(cross(up, N));
	glm::vec3 bitangent = cross(N, tangent);

	glm::vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float a = roughness;
	float k = (a * a) / 2.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(float roughness, float NoV, float NoL)
{
	float ggx2 = GeometrySchlickGGX(NoV, roughness);
	float ggx1 = GeometrySchlickGGX(NoL, roughness);

	return ggx1 * ggx2;
}

glm::vec2 IntegrateBRDF(float NdotV, float roughness, unsigned int samples = 1024)
{
	glm::vec3 V;
	V.x = sqrt(1.0 - NdotV * NdotV);
	V.y = 0.0;
	V.z = NdotV;

	float A = 0.0;
	float B = 0.0;

	glm::vec3 N = glm::vec3(0.0, 0.0, 1.0);

	for (unsigned int i = 0u; i < samples; ++i)
	{
		glm::vec2 Xi = Hammersley(i, samples);
		glm::vec3 H = ImportanceSampleGGX(Xi, roughness, N);
		glm::vec3 L = normalize(2.0f * dot(V, H) * H - V);

		float NoL = glm::max(L.z, 0.0f);
		float NoH = glm::max(H.z, 0.0f);
		float VoH = glm::max(dot(V, H), 0.0f);
		float NoV = glm::max(dot(N, V), 0.0f);

		if (NoL > 0.0)
		{
			float G = GeometrySmith(roughness, NoV, NoL);
			float G_Vis = (G * VoH) / (NoH * NoV) / NoL;
			float Fc = pow(1.0 - VoH, 5.0);

			A += (1.0 - Fc) * G_Vis;
			B += Fc * G_Vis;
		}
	}

	return glm::vec2(A / float(samples), B / float(samples));
}

int main(int argc, char* argv[])
{
	int N = 512;
	gli::texture2d tex = gli::texture2d(gli::FORMAT_RG32_SFLOAT_PACK32, gli::extent2d(N, N), 1);
	for(int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			float NoV = (i + 0.5f) * (1.0f / N);
			float roughness = (j + 0.5f) * (1.0f / N);
			glm::vec2 eval = IntegrateBRDF(NoV, roughness);
			tex.store<glm::vec2>({ i, N - j - 1 }, 0, eval);
		}
	string outdir = "../Textures/BRDFLUT/";
	string outfile = outdir + "BRDFLut.dds";
	try
	{
		cout << "write rendered images: " << outfile << endl;
		gli::save(tex, outfile);
	}
	catch (std::exception e)
	{
		cout << "***** AN ERROR OCCURRED *****" << endl;
		cout << e.what() << endl;
		return 1;
	}
	std::cout << "  [" << N << " x " << N << "] BRDF LUT generated using " << 1024 << " samples.\n";
	std::cout << "Saved LUT to " << outfile << ".\n";
	return 0;
}