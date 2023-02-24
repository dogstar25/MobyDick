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
	
	//Get the doors action component
	const auto& doorActionComponent = doorGameObject.value()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	std::optional<std::shared_ptr<Action>> openCloseAction{};
	std::optional<std::shared_ptr<Action>> enterAction{};

	ImGui::Begin(m_gameObjectType.c_str(), nullptr, m_flags);
	{
		ImGui::PushFont(m_normalFont);
		ImGui::SetWindowPos(ImVec2{ renderComponent->getRenderDestRect().x, renderComponent->getRenderDestRect().y });

		//If the door is open and the door has an "ENTER" action defined, then show the ENTER menu item
		if (doorState == AnimationState::OPENED && doorActionComponent->hasAction(Actions::ENTER)) {
			
			enterAction = doorActionComponent->getAction(Actions::ENTER);
			
			ImGui::SmallButton("W");
			ImGui::SameLine();
			ImGui::Text("ENTER");

		}

		//Open/Close button
		ImGui::SmallButton("E");
		ImGui::SameLine();

		//If the door is open, then make the openCloseAction , the Close action, and visa versa
		if (doorState == AnimationState::OPENED) {
			openCloseAction = doorActionComponent->getAction(Actions::CLOSE);
			ImGui::Text("CLOSE");
		}
		else if (doorState == AnimationState::CLOSED) {
			openCloseAction = doorActionComponent->getAction(Actions::OPEN);
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

	//Execute the open or close action of the door
	if (ImGui::IsKeyPressed(ImGuiKey_E) && openCloseAction.has_value()) {

		openCloseAction.value()->perform(parent()->parent().value());

	}

	//Execute the player ENTERING the door action
	if (ImGui::IsKeyPressed(ImGuiKey_W) && enterAction.has_value()) {

		const auto& player = parent()->parentScene()->getFirstGameObjectByTrait(TraitTag::player);
		enterAction.value()->perform(player->get(), parent()->parent().value());

	}

	return windowSize;
}