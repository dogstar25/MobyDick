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
	inline SDL_Color GRAPE = { 200, 0, 255, 255 };

	//yellow
	inline SDL_Color GOLD = { 173, 167, 54, 255 };

	//blue
	inline SDL_Color MARINE = { 50, 147, 196, 255 };
	inline SDL_Color SAPPHIRE = { 113, 197, 219, 255 };

	//grey / dark
	inline SDL_Color CHARCOAL = { 25, 25, 25, 255 };
	inline SDL_Color MIRKWOOD = { 39, 52, 39, 255 };
	inline SDL_Color SLATE = { 11, 14, 51, 255 };

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
	inline constexpr int DOOR = 18;

}

namespace ComponentTypes {

	inline constexpr int BOBBY_PLAYER_CONTROL_COMPONENT = 27;

}

namespace BrainState {

	inline constexpr auto FOLLOW = 20;

}

namespace TraitTag {

	//Custom traits
}

namespace StatusItemId {
	inline constexpr int PLAYERS_HEART_COUNT = 10;
	
};


