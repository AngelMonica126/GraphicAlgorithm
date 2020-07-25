#pragma once
#include "random"

class CTools
{
public:
	CTools();
	~CTools();
	
public:
	float getRandom01();
	int   getRandomIntRange(int min, int max);

private:
	std::default_random_engine e;
};
