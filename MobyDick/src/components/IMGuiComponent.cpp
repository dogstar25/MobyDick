#include "IMGuiComponent.h"
#include "../game.h"
#include "../IMGui/IMGuiItem.h"
#include <memory>

extern std::unique_ptr<Game> game;

IMGuiComponent::IMGuiComponent(Json::Value componentJSON, std::string gameObjectName)
{
	auto imguiType = componentJSON["imguiType"].asString();
	auto parms = componentJSON["parms"];
	m_IMGuiItem = game->iMGUIFactory()->create(imguiType, gameObjectName, parms);
}

void IMGuiComponent::update()
{


}

void IMGuiComponent::render()
{
	const auto& renderComponment = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	const auto& transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	SDL_FRect destQuad = { renderComponment->getRenderDestRect() };

	glm::vec2 imGuiWindowSize = m_IMGuiItem->render(destQuad);
	transform->setSize(b2Vec2{ imGuiWindowSize.x,imGuiWindowSize.y });


}