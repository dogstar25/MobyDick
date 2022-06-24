#include "IMGuiPauseWindow.h"

#include "Scene.h"
#include <SDL2/SDL.h>
#include "MR_IMGuiUtil.h"
#include "../GameConstants.h"
#include "../MRContextManager.h"


extern std::unique_ptr<Game> game;


IMGuiPauseWindow::IMGuiPauseWindow(std::string gameObjectId, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize) :
	IMGuiItem(gameObjectId, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize)
{
	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;

}

glm::vec2 IMGuiPauseWindow::render()
{
	ImVec2 buttonSize{ ImGui::MRSettings::button1Size};
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

		//Continue Button
		if (ImGui::Button("Continue", ImGui::MRSettings::button1Size)) {
			sendSceneCloseEvent();
		}

		//spacing
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		//Settings Button
		if (ImGui::Button("Settings", ImGui::MRSettings::button1Size)) {
			ImGui::OpenPopup("SettingsModal");
		}

		if (ImGui::BeginPopupModal("SettingsModal", nullptr, m_SettingsModalflags)) {
			settingsModal();
		}

		//spacing
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		//Quit Button
		if (ImGui::Button("Quit", ImGui::MRSettings::button1Size)) {
			sendQuitEvent();
		}

		//spacing
		ImGui::Spacing(); ImGui::Spacing();	ImGui::Spacing(); ImGui::Spacing();	ImGui::Spacing();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();


		windowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

	}

	ImGui::End();

	ImGui::PopStyleColor();

	return windowSize;

}

void IMGuiPauseWindow::settingsModal()
{

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetWindowPos(ImVec2{ center.x - m_settingsModalSize.x / 2, center.y - m_settingsModalSize.y / 2 });
	ImGui::SetWindowSize(m_settingsModalSize);

	//Button Style
	ImGui::PushStyleColor(ImGuiCol_Button, m_buttonColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_buttonHoverColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_buttonActiveColor);

	ImGui::SameLine(24);
	ImGui::BeginGroup();

	//Spacing
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	// Mouse sensitivity setting slider
	ImGui::Text("Mouse Sensitivity");
	static int mouseSensitivity = game->contextMananger()->getMouseSensitivityForGui();
	ImGui::SliderInt("##mouseSensitivity", &mouseSensitivity, 0, 100);

	//Spacing
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	// Sounds Level setting slider
	static int soundvolume = game->contextMananger()->getSoundVolumeForGui();
	ImGui::Text("Sound Volume");
	ImGui::SliderInt("##soundvolume", &soundvolume, 0, 100);

	//Spacing
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	//Buttons
	if (ImGui::Button("Ok", ImGui::MRSettings::button1Size)) {
		apply(mouseSensitivity, soundvolume);
		//ImGui::CloseCurrentPopup();
		sendSceneCloseEvent();
	}

	ImGui::SameLine(156);
	if (ImGui::Button("Cancel", ImGui::MRSettings::button1Size)) {
		//ImGui::CloseCurrentPopup();
		sendSceneCloseEvent();
	}
	ImGui::EndGroup();


	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();


	if (ImGui::IsKeyPressed(ImGuiKey_G)) {
		std::cout << "Settings Modal Key Pressed" << "\n";
	}

	ImGui::EndPopup();

}
void IMGuiPauseWindow::sendSceneCloseEvent()
{

	SDL_Event event;

	SceneAction* sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_EXIT;
	sceneAction->actionId = "";

	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);


}

void IMGuiPauseWindow::sendQuitEvent()
{

	SDL_Event event;

	SceneAction* sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_QUIT;
	sceneAction->actionId = "";

	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);


}

void IMGuiPauseWindow::apply(int mouseSensitivity, int soundVolume)
{

	GameSaveFileData saveFileData{};

	//First load whats currently on file
	std::dynamic_pointer_cast<MRContextManager>(game->contextMananger())->loadGame(saveFileData);

	saveFileData.mouseSensitivity = mouseSensitivity;
	saveFileData.soundLevel = soundVolume;

	std::dynamic_pointer_cast<MRContextManager>(game->contextMananger())->saveGame(saveFileData);

}



