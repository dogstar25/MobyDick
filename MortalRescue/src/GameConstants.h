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
	
	inline constexpr int SMOKE_PARTICLE = 10;
	inline constexpr int NAVIGATION_POINT = 11;
	inline constexpr int HEAVY_PARTICLE = 12;
	inline constexpr int LIGHT_PARTICLE = 13;
	inline constexpr int PLAYER_TOUCH = 14;
	inline constexpr int PLAYER_COLLISION = 15;
	inline constexpr int PLAYER_HITBOX = 16;
	inline constexpr int WALL = 17;
	inline constexpr int DEFLECT_EFFECT = 18;
	inline constexpr int DRONE_BRAIN = 19;
	inline constexpr int DRONE_FRAME = 20;
	inline constexpr int DRONE_SHIELD = 21;
	inline constexpr int DRONE_WEAPON = 22;
	inline constexpr int ENEMY_BULLET = 23;
	inline constexpr int FRIENDLY_BULLET = 24;
	inline constexpr int PLAYER_BULLET = 25;
	inline constexpr int SHIELD_SCRAP = 26;
	inline constexpr int SURVIVOR = 27;
	inline constexpr int MEDKIT = 28;
	inline constexpr int WEAPON_PICKUP = 29;
	inline constexpr int BUTTON = 30;
	inline constexpr int ESCAPE_STAIRS = 31;
	inline constexpr int WALL_PIECE = 32;
	inline constexpr int ENEMY_TURRET = 33;
	inline constexpr int DRONE_SCRAP = 34;
	inline constexpr int HEART_PICKUP = 35;
	inline constexpr int HEALTH_STATION_INSIDE = 36;
	inline constexpr int PLAYER_PULSE_BULLET = 37;
	inline constexpr int PLAYER_PULSE_EXPLOSION = 38;
	inline constexpr int RADIATION_PARTICLE = 39;
	inline constexpr int CHECKPOINT = 40;

}

namespace CollectibleTypes {
	inline constexpr int DRONE_SCRAP = 1;
}

namespace ComponentTypes {

	inline constexpr int BRAIN_DRONE_COMPONENT = 24;
	inline constexpr int WEAPON_PISTOL_COMPONENT = 25;
	inline constexpr int TURRET_BRAIN_COMPONENT = 26;
	inline constexpr int SURVIVOR_BRAIN_COMPONENT = 27;
	inline constexpr int GINA_PLAYER_CONTROL_COMPONENT = 28;
	inline constexpr int GINA_VITALITY_COMPONENT = 29;


}

namespace BrainState {

	inline constexpr auto FOLLOW = 20;
	inline constexpr auto LOST = 21;
	inline constexpr auto ESCAPE = 22;

}

namespace TraitTag {

	//Custom traits
}

namespace StatusItemId {
	inline constexpr int PLAYERS_HEART_COUNT = 10;
	inline constexpr int PLAYER_WEAPON_LEVEL = 11;
	inline constexpr int SCRAP_COUNT = 12;
	inline constexpr int PLAYER_WEAPON_ACCRUAL = 13;
	inline constexpr int SURVIVORS = 14;
	inline constexpr int PLAYER_WEAPON_IS_CHARGED = 15;
	inline constexpr int PLAYER_WEAPON_CHARGED_PERCENT = 16;
	
};


