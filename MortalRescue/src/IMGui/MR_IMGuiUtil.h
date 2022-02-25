#pragma once
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer.h"
#include "game.h"
#include "../GameConstants.h"

namespace ImGui
{
	namespace MRSettings {

		inline static ImVec4 ButtonColor{ MRColors::grey2 };
		inline static ImVec4 ButtonHoverColor{ MRColors::grey1 };
		inline static ImVec4 ButtonActiveColor{ MRColors::green1 };

		inline static ImVec2 button1Size{ 128,32 };

	};

}
