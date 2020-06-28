#include "Globals.h"
#include "EnumMaps.h"

#include <random>
#include <sstream>

#include <box2d/b2_math.h>

static Enums enums;



MouseState::MouseState() {

		m["NONE"] = NONE;
		m["HOVER"] = HOVER;

}

size_t MouseState::toEnum(std::string name) {

	return m[name];
}

WorldObjectCategory::WorldObjectCategory() {

	m["GENERIC"] = GENERIC;
	m["PLAYER"] = PLAYER;
	m["WALL"] = WALL;
	m["PLAYER_BULLET"] = PLAYER_BULLET;
	m["PARTICLE1"] = PARTICLE1;
	m["PARTICLE2"] = PARTICLE2;
	m["PARTICLE3"] = PARTICLE3;
	m["ENEMY_FRAME"] = ENEMY_FRAME;
	m["ENEMY_ARMOR"] = ENEMY_ARMOR;
	m["ENEMY_ARMOR_PIECE"] = ENEMY_ARMOR_PIECE;

}

size_t WorldObjectCategory::toEnum(std::string name) {

	return m[name];
}

namespace util
{

	const int generateRandomNumber(int min, int max)
	{

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(min, max);

		return (int)dist(mt);
	}

	const float generateRandomNumber(float min, float max)
	{

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(min, max);

		return (float)dist(mt);
	}

	const SDL_Color generateRandomColor()
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

}







