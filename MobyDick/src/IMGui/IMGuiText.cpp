#pragma once
#include "IMGuiText.h"
#include "../SoundManager.h"
#include "../TextureManager.h"
#include "../Scene.h"
#include "../game.h"
#include <SDL2/SDL.h>
#include "IMGuiUtil.h"



extern std::unique_ptr<Game> game;


IMGuiText::IMGuiText(Json::Value params, std::string windowName):
	IMGuiItem(params, windowName)
{

	m_label = params["label"].asString();

	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | 
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground;

}

glm::vec2 IMGuiText::render(SDL_FRect destRect)
{
	//reference to io
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//Set style for a textItem
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowMinSize = ImVec2{ 12,12 };
	style.WindowPadding = ImVec2{ 8,8 };
	
	///
	/// BEGIN ImGui Window
	///
	ImGui::Begin(m_name.c_str(), nullptr, m_flags);

	ImGui::SetWindowPos(ImVec2{ destRect.x, destRect.y });

	ImGui::Text(m_label.c_str());

	glm::vec2 windowSize{ ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
	ImGui::End();
	///
	/// END ImGui Window
	///

	return windowSize;


}

