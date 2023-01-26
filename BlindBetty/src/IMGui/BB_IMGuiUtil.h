#pragma once
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer.h"
#include "game.h"
#include "../GameConstants.h"

namespace ImGui
{
	namespace GameSettings {

		inline static ImVec2 button1Size{ 145,36 };

	}


	void continueGameLoad();
	void newGameLoad();


}
