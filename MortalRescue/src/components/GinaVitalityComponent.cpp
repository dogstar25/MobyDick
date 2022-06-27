#include "GinaVitalityComponent.h"
#include "game.h"
#include "../GameConstants.h"

extern std::unique_ptr<Game> game;

GinaVitalityComponent::GinaVitalityComponent()
{

}

GinaVitalityComponent::GinaVitalityComponent(Json::Value componentJSON)
	: VitalityComponent(componentJSON)
{

}

GinaVitalityComponent::~GinaVitalityComponent()
{

}

void GinaVitalityComponent::update()
{

	VitalityComponent::update();

	//Update the player hearts status value 
	game->contextMananger()->setStatusItemValue(StatusItemId::PLAYERS_HEART_COUNT, health());

}
