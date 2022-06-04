 #pragma once
#include "BaseConstants.h"
#include "imgui.h"

namespace Colors {

	inline SDL_Color NOCOLOR = { 0, 0, 0, 0 };

	//green
	inline SDL_Color FOREST = { 10, 75, 10, 255 };
	inline SDL_Color EMERALD = { 10, 125, 10, 255 };
	inline SDL_Color PARAKEET = { 38, 195, 15, 255 };
	
	//red
	inline SDL_Color WINE = { 127, 6, 34, 255 };

	//yellow
	inline SDL_Color GOLD = { 173, 167, 54, 255 };

	//blue
	inline SDL_Color MARINE = { 50, 147, 196, 255 };
	inline SDL_Color SAPPHIRE = { 113, 197, 219, 255 };

	//grey / dark
	inline SDL_Color CHARCOAL = { 25, 25, 25, 255 };
	inline SDL_Color MIRKWOOD = { 39, 52, 39, 255 };

}

namespace ContactTag {
	
	inline constexpr int SMOKE_PARTICLE = 2;
	inline constexpr int NAVIGATION_POINT = 3;
	inline constexpr int HEAVY_PARTICLE = 4;
	inline constexpr int LIGHT_PARTICLE = 5;
	inline constexpr int PLAYER_TOUCH = 6;
	inline constexpr int PLAYER_COLLISION = 7;
	inline constexpr int PLAYER_HITBOX = 8;
	inline constexpr int WALL = 9;
	inline constexpr int DEFLECT_EFFECT = 10;
	inline constexpr int DRONE_BRAIN = 11;
	inline constexpr int DRONE_FRAME = 12;
	inline constexpr int DRONE_SHIELD = 13;
	inline constexpr int DRONE_WEAPON = 14;
	inline constexpr int ENEMY_BULLET = 15;
	inline constexpr int FRIENDLY_BULLET = 16;
	inline constexpr int PLAYER_BULLET = 17;
	inline constexpr int SHIELD_SCRAP = 18;
	inline constexpr int SURVIVOR = 19;
	inline constexpr int MEDKIT = 20;
	inline constexpr int WEAPON_PICKUP = 21;
	inline constexpr int BUTTON = 22;
	inline constexpr int ESCAPE_STAIRS = 23;
	inline constexpr int WALL_PIECE = 24;

}

namespace CollectibleTypes {
	inline constexpr int DRONE_SCRAP = 1;
}

namespace ComponentTypes {

	inline constexpr int BRAIN_DRONE_COMPONENT = 22;
	inline constexpr int WEAPON_PISTOL_COMPONENT = 23;
	inline constexpr int TURRET_BRAIN_COMPONENT = 24;
	inline constexpr int SURVIVOR_BRAIN_COMPONENT = 25;
	inline constexpr int GINA_PLAYER_CONTROL_COMPONENT = 26;
	

}

namespace BrainState {

	inline constexpr auto FOLLOW = 20;
	inline constexpr auto LOST = 21;
	inline constexpr auto ESCAPE = 22;

}

namespace TraitTag {

	//Custom traits
}
