#include "Util.h"

#include <SDL.h>

#include <random>



int Util::generateRandomNumber(int min, int max)
{

	//int r = rand() % 20;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(min, max);

	return (int)dist(mt);



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


Matrix::Matrix() {

}

Matrix::Matrix(int width, int height) {

	///data = new 

}




