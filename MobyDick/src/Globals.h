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

/*
World Object Categories
*/
static struct WorldObjectCategory {
	enum {
		GENERIC = 1,
		PLAYER = 2,
		WALL = 4,
		PLAYER_BULLET = 8,
		PARTICLE1 = 16,
		PARTICLE2 = 32,
		PARTICLE3 = 64,
		ENEMY_FRAME = 128,
		ENEMY_ARMOR = 256,
		ENEMY_ARMOR_PIECE = 512
	};

	WorldObjectCategory();

	size_t toEnum(std::string name);

	std::map<std::string, std::size_t> m;

}WorldObjCat;



/*
Mouse State
*/
struct MouseState{

	enum {
		NONE = 0,
		HOVER = 1,
		HOLD = 2,
		CLICKED = 3
	};

	MouseState();

	size_t toEnum(std::string name);

	std::map<std::string, std::size_t> m;
	
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