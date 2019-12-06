#include "Util.h"

#include <SDL.h>

#include <random>
#include <sstream>
#include <string>

using namespace std;

int Util::generateRandomNumber(int min, int max)
{

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(min, max);

	return (int)dist(mt);
}

float Util::generateRandomNumber(float min, float max)
{

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(min, max);

	return (float)dist(mt);
}

SDL_Color Util::generateRandomColor()
{
	Uint8 red, green, blue, alpha;

	red = generateRandomNumber(0, 255);
	green = generateRandomNumber(0, 255);
	blue = generateRandomNumber(0, 255);
	SDL_Color color;
	color.a = 0;
	color.r = red;
	color.g = green;
	color.b = blue;

	return color;

}

float Util::radiansToDegrees(float angleInRadians)
{
	float angle = angleInRadians / M_PI * 180;
	return angle;

}

string Util::floatToString(float x, int decDigits) 
{
	stringstream ss;
	ss << fixed;
	ss.precision(decDigits); // set # places after decimal
	ss << x;
	return ss.str();
}





