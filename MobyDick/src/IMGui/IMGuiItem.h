#pragma once

#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>

class IMGuiItem
{
public:
	IMGuiItem();
	~IMGuiItem();
	virtual void run() = 0;

private:

};
