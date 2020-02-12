#pragma once
#include <SDL.h>
#include <string>

using namespace std;

namespace util
{

	const int generateRandomNumber(int min, int max);
	const float generateRandomNumber(float min, float max);
	const SDL_Color generateRandomColor();
	const float radiansToDegrees(float angleInRadians);
	const float degreesToRadians(float angleInDegrees);
	const string floatToString(float x, int decDigits);

};



