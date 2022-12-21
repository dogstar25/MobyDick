#include "MREnumMap.h"
#include "GameConstants.h"

MREnumMap::MREnumMap()
{

	//Custom Collision Tags
	addEnumItem("ContactTag::BUTTON", ContactTag::BUTTON);
	addEnumItem("ContactTag::DEFLECT_EFFECT", ContactTag::DEFLECT_EFFECT);
	addEnumItem("ContactTag::DRONE_BRAIN", ContactTag::DRONE_BRAIN);
	addEnumItem("ContactTag::DRONE_SHIELD", ContactTag::DRONE_SHIELD);
	addEnumItem("ContactTag::DRONE_WEAPON", ContactTag::DRONE_WEAPON);
	addEnumItem("ContactTag::DRONE_FRAME", ContactTag::DRONE_FRAME);
	addEnumItem("ContactTag::ENEMY_BULLET", ContactTag::ENEMY_BULLET);
	addEnumItem("ContactTag::FRIENDLY_BULLET", ContactTag::FRIENDLY_BULLET);
	addEnumItem("ContactTag::HEAVY_PARTICLE", ContactTag::HEAVY_PARTICLE);
	addEnumItem("ContactTag::LIGHT_PARTICLE", ContactTag::LIGHT_PARTICLE);
	addEnumItem("ContactTag::PLAYER_COLLISION", ContactTag::PLAYER_COLLISION);;
	addEnumItem("ContactTag::PLAYER_TOUCH", ContactTag::PLAYER_TOUCH);;
	addEnumItem("ContactTag::PLAYER_HITBOX", ContactTag::PLAYER_HITBOX);;
	addEnumItem("ContactTag::PLAYER_BULLET", ContactTag::PLAYER_BULLET);
	addEnumItem("ContactTag::SHIELD_SCRAP", ContactTag::SHIELD_SCRAP);
	addEnumItem("ContactTag::SURVIVOR", ContactTag::SURVIVOR);
	addEnumItem("ContactTag::MEDKIT", ContactTag::MEDKIT);
	addEnumItem("ContactTag::WALL", ContactTag::WALL);
	addEnumItem("ContactTag::WEAPON_PICKUP", ContactTag::WEAPON_PICKUP);
	addEnumItem("ContactTag::ESCAPE_STAIRS", ContactTag::ESCAPE_STAIRS);
	addEnumItem("ContactTag::WALL_PIECE", ContactTag::WALL_PIECE);
	addEnumItem("ContactTag::ENEMY_TURRET", ContactTag::ENEMY_TURRET);
	addEnumItem("ContactTag::HEART_PICKUP", ContactTag::HEART_PICKUP);
	addEnumItem("ContactTag::CONTAINER_INSIDE", ContactTag::HEALTH_STATION_INSIDE);
	addEnumItem("ContactTag::PLAYER_PULSE_BULLET", ContactTag::PLAYER_PULSE_BULLET);
	addEnumItem("ContactTag::PLAYER_PULSE_EXPLOSION", ContactTag::PLAYER_PULSE_EXPLOSION);
	addEnumItem("ContactTag::RADIATION_PARTICLE", ContactTag::RADIATION_PARTICLE);
	addEnumItem("ContactTag::CHECKPOINT", ContactTag::CHECKPOINT);

	//Custom Component Types
	addEnumItem("BRAIN_DRONE_COMPONENT", ComponentTypes::BRAIN_DRONE_COMPONENT);
	addEnumItem("WEAPON_PISTOL_COMPONENT", ComponentTypes::WEAPON_PISTOL_COMPONENT);
	addEnumItem("TURRET_BRAIN_COMPONENT", ComponentTypes::TURRET_BRAIN_COMPONENT);
	addEnumItem("SURVIVOR_BRAIN_COMPONENT", ComponentTypes::SURVIVOR_BRAIN_COMPONENT);
	addEnumItem("GINA_PLAYER_CONTROL_COMPONENT", ComponentTypes::GINA_PLAYER_CONTROL_COMPONENT);
	addEnumItem("GINA_VITALITY_COMPONENT", ComponentTypes::GINA_VITALITY_COMPONENT);

	//Custom Status/Context Item Types
	addEnumItem("StatusItemId::PLAYERS_HEART_COUNT", StatusItemId::PLAYERS_HEART_COUNT);
	addEnumItem("StatusItemId::PLAYER_WEAPON_LEVEL", StatusItemId::PLAYER_WEAPON_LEVEL);
	addEnumItem("StatusItemId::SCRAP_COUNT", StatusItemId::SCRAP_COUNT);
	addEnumItem("StatusItemId::PLAYER_WEAPON_ACCRUAL", StatusItemId::PLAYER_WEAPON_ACCRUAL);
	addEnumItem("StatusItemId::SURVIVORS", StatusItemId::SURVIVORS);
	addEnumItem("StatusItemId::PLAYER_WEAPON_CHARGED_PERCENT", StatusItemId::PLAYER_WEAPON_CHARGED_PERCENT);
	addEnumItem("StatusItemId::PLAYER_WEAPON_IS_CHARGED", StatusItemId::PLAYER_WEAPON_IS_CHARGED);
	addEnumItem("StatusItemId::ENEMY_DRONE_COUNT", StatusItemId::ENEMY_DRONE_COUNT);


}