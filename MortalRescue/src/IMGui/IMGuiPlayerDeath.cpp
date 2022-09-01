#include "IMGuiPlayerDeath.h"

#include "Scene.h"
#include <SDL2/SDL.h>
#include "MR_IMGuiUtil.h"
#include "IMGui/IMGuiUtil.h"
#include "../GameConstants.h"
#include "../MRContextManager.h"


extern std::unique_ptr<Game> game;


IMGuiPlayerDeath::IMGuiPlayerDeath(std::string gameObjectType, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize) :
	IMGuiItem(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize)
{
	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;

}

glm::vec2 IMGuiPlayerDeath::render()
{
	ImVec2 buttonSize{ ImGui::MRSettings::button1Size };
	glm::vec2 windowSize{};

	const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	setWindowProperties(parent());

	//Set Color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, m_backgroundColor);

	ImGui::Begin(m_gameObjectType.c_str(), nullptr, m_flags);
	{

		ImGui::PushStyleColor(ImGuiCol_Button, m_buttonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_buttonHoverColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_buttonActiveColor);

		//top window spacing
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::PushFont(m_xLargeFont);
		ImGui::textCentered("You are Dead!");
		ImGui::Dummy({ 0,48 });
		ImGui::PopFont();

		//Restart Button
		ImGui::setCursorToCentered(ImGui::MRSettings::button1Size.x);
		if (ImGui::Button("Restart Level", ImGui::MRSettings::button1Size)) {
			_sendNextLevelEvent();
		}

		//spacing
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		//Quit Button
		ImGui::setCursorToCentered(ImGui::MRSettings::button1Size.x);
		if (ImGui::Button("Quit", ImGui::MRSettings::button1Size)) {

			util::sendSceneEvent(SCENE_ACTION_EXIT);
			util::sendSceneEvent(SCENE_ACTION_REPLACE, "SCENE_TITLE_SCREEN");
		}


		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();


		windowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

	}

	ImGui::End();

	ImGui::PopStyleColor();

	return windowSize;

}


void IMGuiPlayerDeath::_sendNextLevelEvent()
{


	util::sendSceneEvent(SCENE_ACTION_EXIT);
	util::sendSceneEvent(SCENE_ACTION_LOAD_CURRENTLEVEL);

}






