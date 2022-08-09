#include "IMGuiComponent.h"
#include "../game.h"
#include "../IMGui/IMGuiItem.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer.h"
#include "../ColorMap.h"


#include <memory>

extern std::unique_ptr<Game> game;

IMGuiComponent::IMGuiComponent(Json::Value componentJSON, std::string gameObjectId, Scene* parentScene)
{
	m_componentType = ComponentTypes::IMGUI_COMPONENT;

	auto imguiType = componentJSON["imguiType"].asString();

	ImVec4 color = { 1.0, 1.0, 1.0, 1.0 };
	if (componentJSON.isMember("color")) {
		color = util::JsonToImVec4Color(componentJSON["color"]);
	}

	auto autoSize = componentJSON["autoSize"].asBool();

	b2Vec2 padding = { componentJSON["padding"]["x"].asFloat(), componentJSON["padding"]["y"].asFloat() };
	std::string staticText = componentJSON["staticText"].asString();

	ImVec4 backgroundColor{};
	ImVec4 textColor{};
	ImVec4 buttonColor{};
	ImVec4 buttonHoverColor{};
	ImVec4 buttonActiveColor{};

	if (componentJSON.isMember("color"))
	{

		if (componentJSON["color"].isMember("background")) {
			backgroundColor = game->colorMap()->toIMGUIColor(componentJSON["color"]["background"].asString());

			if (componentJSON["color"].isMember("backgroundAlpha")) {
				util::colorApplyAlpha(backgroundColor, componentJSON["color"]["backgroundAlpha"].asInt());
			}
			else {
				util::colorApplyAlpha(backgroundColor, 255);
			}
		}

		if (componentJSON["color"].isMember("text")) {
			textColor = game->colorMap()->toIMGUIColor(componentJSON["color"]["text"].asString());
		}
		if (componentJSON["color"].isMember("button")) {
			buttonColor = game->colorMap()->toIMGUIColor(componentJSON["color"]["button"].asString());
		}
		if (componentJSON["color"].isMember("buttonHover")) {
			buttonHoverColor = game->colorMap()->toIMGUIColor(componentJSON["color"]["buttonHover"].asString());
		}
		if (componentJSON["color"].isMember("buttonActive")) {
			buttonActiveColor = game->colorMap()->toIMGUIColor(componentJSON["color"]["buttonActive"].asString());
		}
	}


	m_IMGuiItem = game->iMGUIFactory()->create(imguiType, gameObjectId, parentScene, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize, staticText);
}

void IMGuiComponent::setParent(GameObject* parentObject)
{
	//Call base component setParent
	Component::setParent(parentObject);

	//Parent for the IMGuiItem that lives under the interactiveMenuObject
	m_IMGuiItem->setParent(parentObject);


}

void IMGuiComponent::update()
{
	////////////////////////////////
	////NEW TEST TEST
	//const auto& interactActionComponent = getInteractionObject()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	//const auto& interactAction = interactActionComponent->getAction(ACTION_INTERACTION);

	//if (ImGui::IsKeyPressed(ImGuiKey_E)) {
	//	interactAction->perform(getInteractionObject().get(), SDL_SCANCODE_E);
	//}
	//if (ImGui::IsKeyPressed(ImGuiKey_R)) {
	//	interactAction->perform(getInteractionObject().get(), SDL_SCANCODE_R);
	//}
	/////////////////////////////////////
}

void IMGuiComponent::render()
{
	const auto& transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	glm::vec2 imGuiWindowSize = m_IMGuiItem->render();
	transform->setSize(b2Vec2{ imGuiWindowSize.x,imGuiWindowSize.y });


}