#include "IMGuiLevelComplete.h"

#include "Scene.h"
#include <SDL2/SDL.h>
#include "MR_IMGuiUtil.h"
#include "IMGui/IMGuiUtil.h"
#include "../GameConstants.h"
#include "../MRContextManager.h"


extern std::unique_ptr<Game> game;


IMGuiLevelComplete::IMGuiLevelComplete(std::string gameObjectId, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize) :
	IMGuiItem(gameObjectId, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize)
{
	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;

}

glm::vec2 IMGuiLevelComplete::render()
{
	ImVec2 buttonSize{ ImGui::MRSettings::button1Size };
	glm::vec2 windowSize{};

	const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	setWindowProperties(parent());

	//Set Color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, m_backgroundColor);

	ImGui::Begin(m_gameObjectId.c_str(), nullptr, m_flags);
	{

		ImGui::PushStyleColor(ImGuiCol_Button, m_buttonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_buttonHoverColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_buttonActiveColor);

		//top window spacing
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::PushFont(m_xLargeFont);
		ImGui::textCentered("Level Complete!");
		ImGui::Dummy({0,48});
		ImGui::PopFont();

		//Continue Button
		ImGui::setCursorToCentered(ImGui::MRSettings::button1Size.x);
		if (ImGui::Button("Continue", ImGui::MRSettings::button1Size)) {
			_sendNextLevelEvent();
		}

		//spacing
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();


		windowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

	}

	ImGui::End();

	ImGui::PopStyleColor();

	return windowSize;

}


void IMGuiLevelComplete::_sendNextLevelEvent()
{


	//Load the next level into SCENE_PLAY
	//LevelManager::instance().loadNextLevel("SCENE_PLAY");

	//parent()->parentScene()->loadNextLevel();

	SDL_Event event;
	SceneAction* sceneAction{};

	//Push the event from this levelComplete scene
	sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_EXIT;
	sceneAction->actionId = "";

	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);

	//Push the event from this levelComplete scene
	sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_LOAD_NEXTLEVEL;
	sceneAction->actionId = "";

	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);


}



void IMGuiLevelComplete::apply(int mouseSensitivity, int soundVolume)
{

	SaveFileData saveFileData{};

	//First load whats currently on file
	std::dynamic_pointer_cast<MRContextManager>(game->contextMananger())->loadGame(saveFileData);

	saveFileData.mouseSensitivity = mouseSensitivity;
	saveFileData.soundLevel = soundVolume;

	std::dynamic_pointer_cast<MRContextManager>(game->contextMananger())->saveGame(saveFileData);

}



