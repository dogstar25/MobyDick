#pragma once
#include "IMGui/IMGuiItem.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "GameObject.h"
#include <memory>

class IMGuiSettings : public IMGuiItem
{
public:
	IMGuiSettings() = default;
	IMGuiSettings(Json::Value params, std::string windowName);
	glm::vec2 render(SDL_FRect destRect) override;
	void sendSceneCloseEvent();

	ImFont* m_font{};

private:
	//bool show_another_window = true;

};