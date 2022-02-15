#pragma once
#include "IMGuiExample.h"
#include "../SoundManager.h"
#include "../TextureManager.h"
#include "../Scene.h"
#include "../game.h"
#include <SDL2/SDL.h>

extern std::unique_ptr<Game> game;

IMGuiExample::IMGuiExample()
{
}

void IMGuiExample::run()
{

	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);

	//OpenGl Version of displaying an image
	if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

		GLuint textureAtlasId = static_cast<GLRenderer*>(game->renderer())->getTextureId(GL_TextureIndexType::MAIN_TEXTURE_ATLAS);
		ImGui::Image((void*)(int*)textureAtlasId, ImVec2(16, 16), ImVec2(.00122, .000122), ImVec2(.00244, .00244));
	}
	else {

		//SDL2 Texture void* is the SDL_Texture*
		SDL_Texture* sdlTexture = TextureManager::instance().getTexture("TEXTURE_ATLAS_0")->sdlTexture;
		ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(132, 132), ImVec2(.00122, .000122), ImVec2(.00244, .00244));

	}

}

