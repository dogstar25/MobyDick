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

//Change these to consts?
enum {
	//Mouse State
	MOUSE_NONE = 0,
	MOUSE_HOVER = 1,
	MOUSE_HOLD = 2,
	MOUSE_CLICKED = 3,

	//Collision Groups
	COLLISION_GENERIC = 1,
	COLLISION_PLAYER = 2,
	COLLISION_WALL = 4,
	COLLISION_PLAYER_BULLET = 8,
	COLLISION_PARTICLE1 = 16,
	COLLISION_PARTICLE2 = 32,
	COLLISION_PARTICLE3 = 64,
	COLLISION_ENEMY_FRAME = 128,
	COLLISION_ENEMY_ARMOR = 256,
	COLLISION_ENEMY_ARMOR_PIECE = 512,

	//Components
	ANIMATION_COMPONENT = 1,
	ATTACHMENTS_COMPONENT = 2,
	CHILDREN_COMPONENT = 3,
	COMPOSITE_COMPONENT = 4,
	TRANSFORM_COMPONENT = 5,
	PARTICLE_COMPONENT = 6,
	PHYSICS_COMPONENT = 7,
	RENDER_COMPONENT = 8,
	TEXT_COMPONENT = 9,
	VITALITY_COMPONENT = 10,
	WEAPON_COMPONENT = 11,
	PLAYERCONTROL_COMPONENT = 12,

	//Animation States
	ANIMATION_IDLE = 0,
	ANIMATION_RUN = 1, 

	ANIMATION_ACTIVE = 3, 
	ANIMATION_ACTION = 4,

	//Player Control
	CONTROL_MOVEMENT = 1,
	CONTROL_ROTATION = 2



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
enum GameObjectLayer {

	BACKGROUND = 0,
	MAIN = 1,
	TEXT = 2,
	DEBUG = 3,
};





#endif