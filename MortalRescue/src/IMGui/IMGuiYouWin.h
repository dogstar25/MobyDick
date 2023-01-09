#pragma once
#include "IMGui/IMGuiItem.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "GameObject.h"
#include <memory>


class IMGuiYouWin : public IMGuiItem
{
public:
	IMGuiYouWin() = default;
	IMGuiYouWin(std::string gameObjectType, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor, ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor,
		bool autoSize);
	glm::vec2 render() override;
	void sendRestartGameEvent();
	void apply(int mouseSensitivity, int soundVolume);

	ImFont* m_font{};

private:
	ImGuiWindowFlags m_SettingsModalflags{ ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove };

	void _sendRestartGaneEvent();

};

