#include "IMGuiComponent.h"
#include "../IMGui/IMGuiExample.h"
#include "../IMGui/IMGuiFactory.h"
#include "../game.h"
#include <memory>

extern std::unique_ptr<Game> game;

IMGuiComponent::IMGuiComponent(Json::Value componentJSON)
{
	auto imguiId = componentJSON["imguiId"].asString();

	m_IMGuiItem = game->iMGUIFactory()->create(imguiId);
}

void IMGuiComponent::update()
{

}

void IMGuiComponent::render()
{

	m_IMGuiItem->run();

}