#pragma once
#include <SDL.h>

class Util
{

public:

	

	int generateRandomNumber(int min, int max);
	float generateRandomNumber(float min, float max);
	SDL_Color generateRandomColor();
	float radiansToDegrees(float angleInRadians);
	



};

class Matrix
{
	int width;
	int height;
	char* data;

public:
	Matrix();
	Matrix(int width, int height);
	~Matrix();

	char getArrayValue(int row, int col);
	void setArrayValue(int row, int col, char val);

};

