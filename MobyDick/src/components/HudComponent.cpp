#include "HudComponent.h"

#include "../Scene.h"
#include "../hud/HudItemFactory.h"
#include "../EnumMaps.h"
#include "../hud/HudItem.h"


HudComponent::HudComponent(Json::Value componentJSON, Scene* parentScene)
{

	std::string labelObjectId = componentJSON["labelObjectId"].asString();
	std::string statusObjectId = componentJSON["statusObjectId"].asString();
	std::string statusValueId = componentJSON["statusValueId"].asString();
	float labelPadding = componentJSON["labelPadding"].asFloat();
	HudItemTypes type = static_cast<HudItemTypes>(EnumMap::instance().toEnum(componentJSON["hudType"].asString()));

	m_hudItem = HudItemFactory::instance().create(type, labelObjectId, statusObjectId, statusValueId, labelPadding, parentScene);
}


void HudComponent::update()
{

	//Execute specific HudClassType updates
	m_hudItem->update(parent());


}

void HudComponent::render()
{

	m_hudItem->render(parent());

}