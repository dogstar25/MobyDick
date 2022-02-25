#include "IMGuiPauseWindow.h"

#include "Scene.h"
#include "game.h"
#include <SDL2/SDL.h>
#include "MR_IMGuiUtil.h"
#include "../GameConstants.h"


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

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SameLine((destRect.w / 2) - (buttonSize.x / 2));
		if (ImGui::Button("Continue", buttonSize)) {
			sendSceneCloseEvent();
		}
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::SameLine((destRect.w / 2) - (buttonSize.x / 2));

		if (ImGui::Button("Settings", buttonSize)) {
			ImGui::OpenPopup("Settings1");
		}

		if (ImGui::BeginPopupModal("Settings1", nullptr, m_SettingsModalflags)) {
			settingsModal();
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::SameLine((destRect.w / 2) - (buttonSize.x / 2));
		if (ImGui::Button("Quit", buttonSize)) {
			sendQuitEvent();
		}
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		windowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

		ImGui::End();
	}

	return windowSize;

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

void IMGuiPauseWindow::settingsModal()
{

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetWindowPos(ImVec2{ center.x - m_settingsModalSize.x / 2, center.y - m_settingsModalSize.y / 2 });
	ImGui::SetWindowSize(m_settingsModalSize);

	ImGui::Value("Width", ImGui::GetWindowSize().x);
	ImGui::Value("Height", ImGui::GetWindowSize().y);




	static float colorR, colorG, colorB, colorA;


	ImGui::SliderFloat("Red", &colorR, 0, 1);
	ImGui::SliderFloat("Green", &colorG, 0, 1);
	ImGui::SliderFloat("Blue", &colorB, 0, 1);
	ImGui::SliderFloat("Alpha", &colorA, 0, 1);



	ImGui::PushStyleColor(ImGuiCol_Button, MRColors::green1);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, MRColors::green2);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { colorR , colorG, colorB, colorA });

	ImGui::Button("Settings", ImVec2{ 128,32 });

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();


	ImGui::EndPopup();

}

