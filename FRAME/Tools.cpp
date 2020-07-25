#include "Tools.h"
#include <iostream>
#include "InputManager.h"
#include "App.h"
#include "common.h"

CTools::CTools()
{

}

CTools::~CTools()
{
}

//************************************************************************************
//Function:
float CTools::getRandom01()
{
	std::uniform_real_distribution<float> u(0, 1);
	return u(e);
}

//************************************************************************************
//Function:
int  CTools::getRandomIntRange(int min,int max)
{
	std::uniform_int_distribution<int> u(min, max);
	return u(e);
}
