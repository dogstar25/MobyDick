#include "IMGuiInteractiveMenuDoor.h"
#include "IMGui/IMGuiUtil.h"
#include "ContextManager.h"
#include "Util.h"
#include "game.h"
#include <memory>
#include "components/AnimationComponent.h"
#include "../gameConstants.h"



extern std::unique_ptr<Game> game;

IMGuiInteractiveMenuDoor::IMGuiInteractiveMenuDoor(std::string gameObjectType, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize) :
	IMGuiItem(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize)
{

	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;

}

glm::vec2 IMGuiInteractiveMenuDoor::render()
{

	glm::vec2 windowSize{};

	const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	setWindowProperties(parent());

	//Set color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, m_backgroundColor);
	ImGui::PushStyleColor(ImGuiCol_Button, m_buttonColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_buttonHoverColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_buttonActiveColor);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);


	//What is the current state of the door
	const auto& doorGameObject = parent()->parent();
	const auto& doorAnimationComponent = doorGameObject.value()->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
	auto doorState = doorAnimationComponent->currentAnimationState();

	ImGui::Begin(m_gameObjectType.c_str(), nullptr, m_flags);
	{
		ImGui::PushFont(m_normalFont);
		ImGui::SetWindowPos(ImVec2{ renderComponent->getRenderDestRect().x, renderComponent->getRenderDestRect().y });

		//Walk thru kety if its open
		if (doorState == AnimationState::OPENED && doorGameObject.value()->type() != "DOOR_SIDE") {
			ImGui::SmallButton("W");
			ImGui::SameLine();
			ImGui::Text("ENTER");

		}

		//Open/Close button
		ImGui::SmallButton("E");
		ImGui::SameLine();

		if (doorState == AnimationState::OPENED) {
			ImGui::Text("CLOSE");
		}
		else if (doorState == AnimationState::CLOSED) {
			ImGui::Text("OPEN");
		}

		ImGui::PopFont();
		ImGui::SameLine();

		windowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
	}
	ImGui::End();

	//pop color style
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	//Handle executing the interActionAction tied to what the user selects
	//The interaction object at this point, needed by the interactAction will always be the player at this point
	const auto& player = parent()->parentScene()->getFirstGameObjectByTrait(TraitTag::player);
	

	if (player.has_value() && doorGameObject.has_value()) {

		const auto& baseObjectActionComponent = doorGameObject.value()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
		const auto& interactAction = baseObjectActionComponent->getAction(ACTION_INTERACTION);

		if (ImGui::IsKeyPressed(ImGuiKey_E)) {

			interactAction->perform(player->get(), parent()->parent().value(), SDL_SCANCODE_E);
		}
		if (doorState == AnimationState::OPENED) {

			if (ImGui::IsKeyPressed(ImGuiKey_W) && doorGameObject.value()->type() != "DOOR_SIDE") {

				interactAction->perform(player->get(), parent()->parent().value(), SDL_SCANCODE_W);
			}
		}

	}

	return windowSize;
}