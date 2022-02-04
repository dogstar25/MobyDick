#pragma once
#include "IMGuiSettingsMenu.h"

#include "../Scene.h"

#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"


IMGuiSettingsMenu::IMGuiSettingsMenu()
{

}

void IMGuiSettingsMenu::run()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//bool show_demo_window = true;
	//ImGui::ShowDemoWindow(&show_demo_window);

	
	if (show_another_window) {
		ImGui::Begin("Another Window", &show_another_window, ImGuiWindowFlags_NoTitleBar);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me")) {
			sendSceneCloseEvent();
			show_another_window = false;
		}
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//SDL_GL_SwapWindow(window);


}


void IMGuiSettingsMenu::sendSceneCloseEvent()
{

	SDL_Event event;

	SceneAction* sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_EXIT;
	sceneAction->actionId = "";

	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);


}

