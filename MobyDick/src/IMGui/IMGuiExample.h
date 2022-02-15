#pragma once
#include "IMGuiItem.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include <memory>

class IMGuiExample : public IMGuiItem
{
public:
	IMGuiExample();
	void run() override;
	void sendSceneCloseEvent();

	ImFont* m_font{};

private:
	bool show_another_window = true;

};