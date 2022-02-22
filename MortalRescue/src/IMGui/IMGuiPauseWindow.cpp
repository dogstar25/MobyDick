#include "IMGuiPauseWindow.h"

#include "Scene.h"
#include "game.h"
#include <SDL2/SDL.h>


extern std::unique_ptr<Game> game;


IMGuiPauseWindow::IMGuiPauseWindow(Json::Value params, std::string windowName) :
	IMGuiItem(params, windowName)
{
	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

}

glm::vec2 IMGuiPauseWindow::render(SDL_FRect destRect)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	

	ImVec2 buttonSize{ 128,32 };
	static const ImVec4 color1{ 0.0, 0, 0, 1.0f };
	static const ImVec4 color2{ 0.0, 0, 0, 0.2f };
	static const ImVec4 color3{ 0.0, 0.2, 0, 0.8f };
	static const ImVec4 color4{ 1, 1, 1, 0.5f };


	ImGui::SetNextWindowSize(ImVec2{ destRect.w, destRect.h });

	bool show = true;
	ImGui::Begin(m_name.c_str(), &show, m_flags);

	ImGui::SetWindowPos(ImVec2{ destRect.x, destRect.y });

	//ImGui::PushStyleColor(ImGuiCol_Button,color1);
	ImGui::PushStyleColor(ImGuiCol_Button, color2);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color1);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, color3);

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
	//ImGui::PopStyleColor(ImGuiCol_ButtonHovered);

	glm::vec2 windowSize{ ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

	ImGui::End();

	//glm::vec2  windowSize = { 0,0 };
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
	ImGui::SetWindowPos(ImVec2{ center.x- m_settingsModalSize.x/2, center.y- m_settingsModalSize.y/2 });
	ImGui::SetWindowSize(m_settingsModalSize);

	ImGui::Value("Width", ImGui::GetWindowSize().x);
	ImGui::Value("Height", ImGui::GetWindowSize().y);

	ImGui::EndPopup();

}

