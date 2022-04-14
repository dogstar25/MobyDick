#include "IMGuiHintBasic.h"
#include "IMGuiUtil.h"
#include "../ContextManager.h"
#include "../Util.h"
#include "../game.h"
#include <memory>


extern std::unique_ptr<Game> game;

IMGuiHintBasic::IMGuiHintBasic(std::string gameObjectId, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize) :
	IMGuiItem(gameObjectId, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize)
{

	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;

}

glm::vec2 IMGuiHintBasic::render(GameObject* parentGameObject)
{

	glm::vec2 windowSize{};

	const auto& renderComponent = parentGameObject->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	setWindowProperties(parentGameObject);

	//Set color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, m_backgroundColor);
	ImGui::PushStyleColor(ImGuiCol_Button, m_buttonColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_buttonHoverColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_buttonActiveColor);


	ImGui::Begin(m_gameObjectId.c_str(), nullptr, m_flags);
	{
		ImGui::PushFont(m_smallFont);
		ImGui::SetWindowPos(ImVec2{ renderComponent->getRenderDestRect().x, renderComponent->getRenderDestRect().y });
		ImGui::SmallButton("E");
		ImGui::SameLine();

		ImGui::Text("Use");
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

	return windowSize;
}