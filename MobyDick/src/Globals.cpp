#include "Globals.h"

#include <random>

#include <box2d/b2_math.h>



namespace util
{

	const int generateRandomNumber(int min, int max)
	{

		if (min == max) {
			return min;
		}


		/*srand((unsigned)time(0));
		int randomNumber;
		randomNumber = (rand() % max) + min;*/


		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(min, max);

		return (int)dist(mt);
	}

	const float generateRandomNumber(float min, float max)
	{
		if (min == max) {
			return min;
		}

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(min, max);

		return (float)dist(mt);
	}

	const SDL_Color generateRandomColor()
	{
		Uint8 red, green, blue;

		red = generateRandomNumber(0, 255);
		green = generateRandomNumber(0, 255);
		blue = generateRandomNumber(0, 255);
		SDL_Color color;
		color.a = 255;
		color.r = red;
		color.g = green;
		color.b = blue;

		return color;

	}

	const SDL_Color generateRandomColor(SDL_Color beginRange, SDL_Color endRange)
	{
		Uint8 red, green, blue;

		red = generateRandomNumber(beginRange.r, endRange.r);
		green = generateRandomNumber(beginRange.g, endRange.g);
		blue = generateRandomNumber(beginRange.b, endRange.b);
		SDL_Color color = {red, green, blue, 255};

		return color;

	}

	const float radiansToDegrees(float angleInRadians)
	{
		float angle = angleInRadians / b2_pi * 180;
		return angle;

	}

	const float degreesToRadians(float angleInDegrees)
	{
		float angle = angleInDegrees * 0.0174532925199432957f;
		return angle;

	}

	const std::string floatToString(float x, int decDigits)
	{
		std::stringstream ss;
		ss << std::fixed;
		ss.precision(decDigits); // set # places after decimal
		ss << x;
		return ss.str();
	}

	const SDL_Color JsonToColor(Json::Value JsonColor) {

		SDL_Color color;
		color.r = JsonColor["red"].asInt();
		color.b= JsonColor["blue"].asInt();
		color.g = JsonColor["green"].asInt();
		color.a = JsonColor["alpha"].asInt();

		return color;

	}

}







