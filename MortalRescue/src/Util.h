#pragma once
#include <SDL.h>


class Util
{

public:

	int generateRandomNumber(int min, int max);
	SDL_Color generateRandomColor();
	



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

