#include "PlayerControlComponent.h"


#include "../IMGui/IMGuiUtil.h"
#include "../SceneManager.h"
#include "../EnumMap.h"
#include "../game.h"

extern std::unique_ptr<Game> game;

PlayerControlComponent::PlayerControlComponent()
{

}

PlayerControlComponent::PlayerControlComponent(Json::Value componentJSON)
{
	m_componentType = ComponentTypes::PLAYER_CONTROL_COMPONENT;

}

PlayerControlComponent::~PlayerControlComponent()
{

}






