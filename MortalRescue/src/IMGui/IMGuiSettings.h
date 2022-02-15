#pragma once
#include "IMGui/IMGuiItem.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include <memory>

class IMGuiSettings : public IMGuiItem
{
public:
	IMGuiSettings();
	void run() override;
	void sendSceneCloseEvent();

	ImFont* m_font{};

private:
	//bool show_another_window = true;

};