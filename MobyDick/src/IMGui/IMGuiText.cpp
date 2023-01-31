#pragma once
#include "IMGuiText.h"
#include "../SoundManager.h"
#include "../TextureManager.h"
#include "../Scene.h"
#include "../game.h"
#include <SDL2/SDL.h>
#include "IMGuiUtil.h"



extern std::unique_ptr<Game> game;


IMGuiText::IMGuiText(std::string gameObjectType, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor, ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize, std::string staticTextValue):
	IMGuiItem(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize)
{

	m_staticTextValue = staticTextValue;

	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground;

}

glm::vec2 IMGuiText::render()
{
	glm::vec2 windowSize{};

	const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	setWindowProperties(parent());

	//Set color
	ImGui::PushStyleColor(ImGuiCol_Text, m_textColor);

	//Build the window
	ImGui::Begin(m_gameObjectType.c_str(), nullptr, m_flags);
	{
		ImGui::SetWindowPos(ImVec2{ renderComponent->getRenderDestRect().x, renderComponent->getRenderDestRect().y });

		ImGui::Text(m_staticTextValue.c_str());

		windowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

	}
	ImGui::End();

	//pop color style
	ImGui::PopStyleColor();
	//

	return windowSize;


}

