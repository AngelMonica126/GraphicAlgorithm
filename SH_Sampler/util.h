#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

const float PI = (float)M_PI;

class Vec3
{
public:	
	Vec3(float x_, float y_, float z_)
		:x(x_), y(y_), z(z_)
	{}
	Vec3()
		:x(0), y(0), z(0)
	{}
	union { float x, r, radius; };
	union { float y, g, theta; };
	union { float z, b, phi; };

};

inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
	return Vec3(a.x+b.x, a.y+b.y, a.z+b.z);
}

inline Vec3 operator*(float c, const Vec3& a)
{
	return Vec3(c*a.x, c*a.y, c*a.z);
}

inline Vec3 operator/(const Vec3& a, float c)
{
	return Vec3(a.x/c, a.y/c, a.z/c);
}

inline Vec3 Normalize(const Vec3& a)
{
	float r = std::sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
	return { a.x / r, a.y / r, a.z / r };
}

class Vertex
{
public:
	Vec3 pos, color;
};

inline Vec3 Cartesian2Spherical(const Vec3& p)
{
	Vec3 s;
	s.radius = std::sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
	s.theta = std::acos(p.y / s.radius);
	s.phi = std::atan2(p.z, p.x);
	return s;
}

inline Vec3 Spherical2Cartesian(const Vec3& s)
{
	Vec3 p;
	p.x = s.radius*std::sin(s.theta)*std::cos(s.phi);
	p.y = s.radius*std::cos(s.theta);
	p.z = s.radius*std::sin(s.theta)*std::sin(s.phi);
	return p;
}

struct CubeUV
{
	int index;
	float u, v;
};

inline Vec3 CubeUV2XYZ(const CubeUV& c)
{
	float u = c.u*2.f - 1.f;
	float v = c.v*2.f - 1.f;
	switch (c.index)
	{
	case 0: return {  1,  v, -u }; 	// +x
	case 1: return { -1,  v,  u }; 	// -x
	case 2: return {  u,  1, -v };  // +y
	case 3: return {  u, -1,  v };	// -y
	case 4: return {  u,  v,  1 };  // +z
	case 5: return { -u,  v, -1 };	// -z
	}
	return Vec3();
}

inline CubeUV XYZ2CubeUV(const Vec3& p)
{
	float ax = std::abs(p.x);
	float ay = std::abs(p.y);
	float az = std::abs(p.z);
	CubeUV c;
	if (ax >= ay && ax >= az)	// x face
	{
		c = { p.x >= 0 ? 0 : 1, -p.z / p.x, p.y / ax };
	}
	else if (ay >= az)	// y face
	{
		c = { p.y >= 0 ? 2 : 3, p.x / ay, -p.z / p.y };
	}
	else // z face
	{
		c = { p.z >= 0 ? 4 : 5, p.x / p.z, p.y / az };
	}
	c.u = c.u*0.5f + 0.5f;
	c.v = c.v*0.5f + 0.5f;
	return c;
}

static double surfaceArea(double x, double y)
{
	return atan2(x * y, sqrt(x * x + y * y + 1.0));
}

inline float UniformRandom()
{
	static std::default_random_engine generator;
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	return distribution(generator);
}

inline float NormalRandom(float mu = 0.f, float sigma = 1.f)
{
	static std::default_random_engine generator;
	static std::normal_distribution<float> distribution(mu, sigma);
	return distribution(generator);
}