#pragma once

#include "IMGui/IMGuiItem.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "GameObject.h"
#include <memory>


class IMGuiTopHud : public IMGuiItem{

public:
	IMGuiTopHud() = default;
	IMGuiTopHud(Json::Value params, std::string windowName);
	glm::vec2 render(SDL_FRect destRect) override;

private:

	void hudScrapCount();
	void hudScrapBar();

};
