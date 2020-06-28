#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <map>

#include <SDL2/SDL_pixels.h>


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

namespace constants
{

	static constexpr int MAX_GAMEOBJECT_LAYERS = 4;
	static constexpr short CHILD_POSITIONS = 9;

}

namespace util
{

	const int generateRandomNumber(int min, int max);
	const float generateRandomNumber(float min, float max);
	const SDL_Color generateRandomColor();
	const float radiansToDegrees(float angleInRadians);
	const float degreesToRadians(float angleInDegrees);
	const std::string floatToString(float x, int decDigits);

};

enum {
	MOUSE_NONE = 0,
	MOUSE_HOVER = 1,
	MOUSE_HOLD = 2,
	MOUSE_CLICKED = 3,

	COLLISION_GENERIC = 1,
	COLLISION_PLAYER = 2,
	COLLISION_WALL = 4,
	COLLISION_PLAYER_BULLET = 8,
	COLLISION_PARTICLE1 = 16,
	COLLISION_PARTICLE2 = 32,
	COLLISION_PARTICLE3 = 64,
	COLLISION_ENEMY_FRAME = 128,
	COLLISION_ENEMY_ARMOR = 256,
	COLLISION_ENEMY_ARMOR_PIECE = 512
};

/*
Game State
*/
enum GameState{ 
	QUIT = 0,
	PLAY = 1,
	PAUSE = 2

};


/*
Game Object Layers
*/
enum GameOjectLayer {

	BACKGROUND = 0,
	MAIN = 1,
	TEXT = 2,
	DEBUG = 3,
};





#endif