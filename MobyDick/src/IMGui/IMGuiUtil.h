#pragma once
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer.h"
#include "../game.h"

namespace ImGui
{
	void MobyDickInit(Game*);
	void MobyDickNewFrame();
	void MobyDickRenderFrame();
	void MobyDickFPSFrame();
	
}
