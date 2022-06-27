#pragma once
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer.h"
#include "../game.h"

namespace ImGui
{
	namespace settings {


	}


	void MobyDickInit(Game*);
	void MobyDickNewFrame();
	void MobyDickRenderFrame();
	void MobyDickFPSFrame();
	void setFont64();
	void textCentered(std::string text);
	void sendQuitEvent();
	void sendSceneExitEvent();
	void setCursorToCentered(float itemToCenterSize);

}
