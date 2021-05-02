#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <map>

#include <json/json.h>
#include <SDL2/SDL.h>
#include <box2d/box2d.h>

/*
Overloaded operators used throught the game
*/
static bool operator ==(SDL_Color a, SDL_Color b)
{
	return (a.r == b.r) && (a.g == b.g) && (a.b == b.b);
}

static bool operator !=(SDL_Color a, SDL_Color b)
{
	return (a.r != b.r) || (a.g != b.g) || (a.b != b.b);
}



namespace util
{
	const int generateRandomNumber(int min, int max);
	const float generateRandomNumber(float min, float max);
	const SDL_Color generateRandomColor();
	const SDL_Color generateRandomColor(SDL_Color beginRange, SDL_Color endRange);
	const float radiansToDegrees(float angleInRadians);
	const float degreesToRadians(float angleInDegrees);
	const std::string floatToString(float x, int decDigits);
	const SDL_Color JsonToColor(Json::Value JsonColor);
	float normalizeRadians(float angleInRadians);
	float normalizeDegrees(float angleInDegrees);
	b2Vec2& toBox2dPoint(b2Vec2& point);
	b2Vec2& toRenderPoint(b2Vec2& point);

	b2Vec2 matchParentRotation(SDL_FPoint childPosition, SDL_FPoint parentPosition, float);

};

/*
Game State
*/
enum class GameState{ 
	QUIT = 0,
	PLAY = 1,
	PAUSE = 2,
	EXIT = 3
};

enum class SceneState {
	RUN = 0,
	PAUSE = 1,
	EXIT = 2
};

namespace WindowPosition {

	inline constexpr int CENTER = 0;
	inline constexpr int TOP_LEFT = 1;
	inline constexpr int TOP_CENTER = 2;
	inline constexpr int TOP_RIGHT = 3;
}



#endif