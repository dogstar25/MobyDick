#pragma once
#include <json/json.h>
#include "../GameObject.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"


class IMGuiItem
{
public:
	IMGuiItem() {};
	IMGuiItem(Json::Value paramsJSON, std::string gameObjectName);

	virtual glm::vec2 render(SDL_FRect destRect) = 0;
	std::string name() { return m_name; }

protected:

	Json::Value m_params{};
	std::string m_name{};

	ImGuiWindowFlags m_flags{};
		
};
