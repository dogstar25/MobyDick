#include "IMGuiComponent.h"
#include "../game.h"
#include "../IMGui/IMGuiItem.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer.h"

#include <memory>

extern std::unique_ptr<Game> game;

IMGuiComponent::IMGuiComponent(Json::Value componentJSON, std::string gameObjectId)
{
	auto imguiType = componentJSON["imguiType"].asString();

	ImVec4 color = { 1.0, 1.0, 1.0, 1.0 };
	if (componentJSON.isMember("color")) {
		color = util::JsonToImVec4Color(componentJSON["color"]);
	}

	auto autoSize = componentJSON["autoSize"].asBool();

	b2Vec2 padding = { componentJSON["padding"]["x"].asFloat(), componentJSON["padding"]["y"].asFloat() };
	std::string staticText = componentJSON["staticText"].asString();

	m_IMGuiItem = game->iMGUIFactory()->create(imguiType, gameObjectId, padding, color, autoSize, staticText);
}

void IMGuiComponent::update()
{


}

void IMGuiComponent::render()
{
	const auto& transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	glm::vec2 imGuiWindowSize = m_IMGuiItem->render(parent());
	transform->setSize(b2Vec2{ imGuiWindowSize.x,imGuiWindowSize.y });


}