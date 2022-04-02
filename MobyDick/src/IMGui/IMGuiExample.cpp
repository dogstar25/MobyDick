#pragma once
#include "IMGuiExample.h"
#include "../TextureManager.h"
#include "../game.h"
#include <SDL2/SDL.h>

extern std::unique_ptr<Game> game;

glm::vec2 IMGuiExample::render(GameObject* parentGameObject)
{

	ImGui::ShowDemoWindow();

	//OpenGl Version of displaying an image
	if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

		GLuint textureAtlasId = static_cast<GLRenderer*>(game->renderer())->getTextureId(GL_TextureIndexType::MAIN_TEXTURE_ATLAS);
		ImGui::Image((void*)(int*)textureAtlasId, ImVec2(16, 16), ImVec2(.00122, .000122), ImVec2(.00244, .00244));
	}
	else {

		//SDL2 Texture void* is the SDL_Texture*
		SDL_Texture* sdlTexture = TextureManager::instance().getTexture("TEXTURE_ATLAS_0")->sdlTexture;
		//ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(132, 132), ImVec2(.00122, .000122), ImVec2(.00244, .00244));
		ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(64, 64), ImVec2(.0009765, .048828), ImVec2(.01635, .0642));
		//ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(64, 64), ImVec2(4/4096, 200/4096), ImVec2(67/4096, 263/4096));

	}


	glm::vec2 windowSize{ ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
	return windowSize;


}

