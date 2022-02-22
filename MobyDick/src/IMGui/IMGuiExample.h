#pragma once
#include "IMGuiItem.h"
#include "../GameObject.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include <memory>



class IMGuiExample : public IMGuiItem
{
public:
	IMGuiExample() = default;
	IMGuiExample(Json::Value params, std::string windowName) :
		IMGuiItem(params, windowName) {};
	glm::vec2 render(SDL_FRect destRect) override;

	ImFont* m_font{};

private:
	bool show_another_window = true;

};