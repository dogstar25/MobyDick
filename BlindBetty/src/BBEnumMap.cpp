#include "BBEnumMap.h"
#include "GameConstants.h"

BBEnumMap::BBEnumMap()
{

	//Custom Collision Tags
	addEnumItem("ContactTag::HEAVY_PARTICLE", ContactTag::HEAVY_PARTICLE);
	addEnumItem("ContactTag::LIGHT_PARTICLE", ContactTag::LIGHT_PARTICLE);
	addEnumItem("ContactTag::PLAYER_COLLISION", ContactTag::PLAYER_COLLISION);
	addEnumItem("ContactTag::PLAYER_TOUCH", ContactTag::PLAYER_TOUCH);
	addEnumItem("ContactTag::PLAYER_HITBOX", ContactTag::PLAYER_HITBOX);
	addEnumItem("ContactTag::WALL", ContactTag::WALL);
	addEnumItem("ContactTag::DOOR_TOUCH", ContactTag::DOOR_TOUCH);
	addEnumItem("ContactTag::DOOR_COLLISION", ContactTag::DOOR_COLLISION);
	addEnumItem("ContactTag::WARP_ENTRY", ContactTag::WARP_ENTRY);
	addEnumItem("ContactTag::STAIRS_TOUCH", ContactTag::STAIRS_TOUCH);
	addEnumItem("ContactTag::STAIRS_TOP_TOUCH", ContactTag::STAIRS_TOP_TOUCH);
	addEnumItem("ContactTag::STAIRS_BOTTOM_TOUCH", ContactTag::STAIRS_BOTTOM_TOUCH);
	

	//Custom Component Types
	addEnumItem("BOBBY_PLAYER_CONTROL_COMPONENT", ComponentTypes::BOBBY_PLAYER_CONTROL_COMPONENT);

	//Custom Status/Context Item Types
	addEnumItem("StatusItemId::PLAYERS_HEART_COUNT", StatusItemId::PLAYERS_HEART_COUNT);

	//Custom animation states
	addEnumItem("AnimationState::WALK_LEFT", AnimationState::WALK_LEFT);
	addEnumItem("AnimationState::WALK_RIGHT", AnimationState::WALK_RIGHT);
	addEnumItem("AnimationState::IDLE_LEFT", AnimationState::IDLE_LEFT);
	addEnumItem("AnimationState::IDLE_RIGHT", AnimationState::IDLE_RIGHT);
	addEnumItem("AnimationState::OPEN", AnimationState::OPEN);
	addEnumItem("AnimationState::CLOSE", AnimationState::CLOSE);
	addEnumItem("AnimationState::OPENED", AnimationState::OPENED);
	addEnumItem("AnimationState::CLOSED", AnimationState::CLOSED);
	addEnumItem("AnimationState::WALK_DOWN", AnimationState::WALK_DOWN);
	addEnumItem("AnimationState::WALK_UP_IDLE", AnimationState::WALK_UP_IDLE);
	addEnumItem("AnimationState::WALK_DOWN_IDLE", AnimationState::WALK_DOWN_IDLE);

	//Custom actions
	m_enumMap["Actions::WARP"] = Actions::WARP;
	m_enumMap["Actions::OPEN"] = Actions::OPEN;
	m_enumMap["Actions::CLOSE"] = Actions::CLOSE;
	m_enumMap["Actions::ENTER"] = Actions::ENTER;
	m_enumMap["Actions::STAIRS_MOVE"] = Actions::STAIRS_MOVE;

	
}