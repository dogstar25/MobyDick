#include "IMGuiPauseWindow.h"

#include "Scene.h"
#include <SDL2/SDL.h>
#include "MR_IMGuiUtil.h"
#include "../GameConstants.h"
#include "../MRContextManager.h"


extern std::unique_ptr<Game> game;


IMGuiPauseWindow::IMGuiPauseWindow(Json::Value params, std::string windowName) :
	IMGuiItem(params, windowName)
{
	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

}

glm::vec2 IMGuiPauseWindow::render(SDL_FRect destRect)
{
	glm::vec2 windowSize{ 1,1 };

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	ImVec2 buttonSize{ ImGui::MRSettings::button1Size};

	//Window Size
	ImGui::SetNextWindowSize(ImVec2{ destRect.w, destRect.h });

	bool show = true;
	{
		ImGui::Begin(m_name.c_str(), &show, m_flags);

		ImGui::SetWindowPos(ImVec2{ destRect.x, destRect.y });

		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::MRSettings::ButtonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::MRSettings::ButtonHoverColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::MRSettings::ButtonActiveColor);

		//top window spacing
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		ImGui::SameLine((destRect.w - ImGui::MRSettings::button1Size.x) / 2);
		ImGui::BeginGroup();

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

		ImGui::EndGroup();

		windowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

		ImGui::End();
	}

	return windowSize;

}

void IMGuiPauseWindow::settingsModal()
{

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetWindowPos(ImVec2{ center.x - m_settingsModalSize.x / 2, center.y - m_settingsModalSize.y / 2 });
	ImGui::SetWindowSize(m_settingsModalSize);

	//Button Style
	ImGui::PushStyleColor(ImGuiCol_Button, MRColors::green1);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, MRColors::green2);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, MRColors::green1);

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
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

	//Buttons
	if (ImGui::Button("Ok", ImGui::MRSettings::button1Size)) {
		apply(mouseSensitivity, soundvolume);
		ImGui::CloseCurrentPopup();
	}

	ImGui::SameLine(156);
	if (ImGui::Button("Cancel", ImGui::MRSettings::button1Size)) {
		ImGui::CloseCurrentPopup();
	}
	ImGui::EndGroup();


	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

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

	SaveFileData saveFileData{};

	//First load whats currently on file
	std::dynamic_pointer_cast<MRContextManager>(game->contextMananger())->loadGame(saveFileData);

	saveFileData.mouseSensitivity = mouseSensitivity;
	saveFileData.soundLevel = soundVolume;

	std::dynamic_pointer_cast<MRContextManager>(game->contextMananger())->saveGame(saveFileData);

}



