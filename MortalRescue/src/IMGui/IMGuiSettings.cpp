#pragma once
#include "IMGuiSettings.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "Scene.h"
#include "game.h"
#include <SDL2/SDL.h>


extern std::unique_ptr<Game> game;


IMGuiSettings::IMGuiSettings(Json::Value params, std::string windowName) :
	IMGuiItem(params, windowName)
{
}

glm::vec2 IMGuiSettings::render(SDL_FRect destRect)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= io.WantCaptureMouse;
	io.ConfigFlags |= io.WantCaptureKeyboard;

	ImGui::Begin("SettingsWindow");
		
	ImGui::Text("Hello from another window!");
	if (ImGui::Button("Close Me")) {
		sendSceneCloseEvent();
	}

	ImGui::SliderFloat("Sound", &SoundManager::instance().test, 0.0f, 10.0f, "ratio = %.3f");

	static char name[128];
	ImGui::InputText("name", name, IM_ARRAYSIZE(name));

	//Image
	if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

		GLuint textureAtlasId = static_cast<GLRenderer*>(game->renderer())->getTextureId(GL_TextureIndexType::MAIN_TEXTURE_ATLAS);
		ImGui::Image((void*)(int*)textureAtlasId, ImVec2(16, 16), ImVec2(.00122, .000122), ImVec2(.00244, .00244));
	}
	else {

		//SDL2 Texture void* is the SDL_Texture*
		SDL_Texture* sdlTexture = TextureManager::instance().getTexture("TEXTURE_ATLAS_0")->sdlTexture;
		ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(132, 132), ImVec2(.00122, .000122), ImVec2(.00244, .00244));
	}

	glm::vec2 windowSize{ ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

	ImGui::End();

	return windowSize;


}

void IMGuiSettings::sendSceneCloseEvent()
{

	SDL_Event event;

	SceneAction* sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_EXIT;
	sceneAction->actionId = "";

	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);


}

