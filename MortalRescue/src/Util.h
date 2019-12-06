#pragma once
#include <SDL.h>
#include <string>

using namespace std;

class Util
{

public:

	int generateRandomNumber(int min, int max);
	float generateRandomNumber(float min, float max);
	SDL_Color generateRandomColor();
	float radiansToDegrees(float angleInRadians);
	string floatToString(float x, int decDigits);

};



