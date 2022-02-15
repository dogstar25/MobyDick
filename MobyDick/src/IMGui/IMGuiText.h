#pragma once
#include "IMGuiItem.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include <memory>

class IMGuiText : public IMGuiItem
{
public:
	IMGuiText();
	void run() override;

	ImFont* m_font{};

private:
	bool show_another_window = true;

};