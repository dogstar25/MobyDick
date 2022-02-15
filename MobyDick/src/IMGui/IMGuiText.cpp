#pragma once
#include "IMGuiText.h"
#include "../SoundManager.h"
#include "../TextureManager.h"
#include "../Scene.h"
#include "../game.h"
#include <SDL2/SDL.h>


extern std::unique_ptr<Game> game;


IMGuiText::IMGuiText()
{

}

void IMGuiText::run()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;


	//ImFont* m_font = io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 22.0f);


	if (GameConfig::instance().rendererType() == RendererType::OPENGL) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
	}
	else {
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
	}

	ImGui::NewFrame();

	ImGui::Begin("test", nullptr, ImGuiWindowFlags_NoTitleBar);

	ImGui::Text("Player Shazam");

	static float testz;
	ImGui::SliderFloat("Sound", &SoundManager::instance().test, 0.0f, 10.0f, "ratio = %.3f");

	ImGui::End();



	// Rendering
	ImGui::Render();
	if (GameConfig::instance().rendererType() == RendererType::OPENGL) {
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	else {
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	}


}

