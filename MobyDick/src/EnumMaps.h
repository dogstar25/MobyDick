#ifndef ENUM_MAPS_H
#define ENUM_MAPS_H

#include "Globals.h"

class Enums {

public:
	Enums();
	~Enums();

	enum{
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

	std::map<std::string, size_t> enumMap;

	size_t toEnum(std::string name, std::map<std::string, size_t> enumMap);

};

#endif

