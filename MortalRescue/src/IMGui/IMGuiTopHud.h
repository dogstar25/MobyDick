#pragma once

#include "IMGui/IMGuiItem.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "GameObject.h"
#include "Scene.h"
#include <memory>


class IMGuiTopHud : public IMGuiItem{

public:
	IMGuiTopHud() = default;
	IMGuiTopHud(std::string gameObjectId, b2Vec2 padding, Scene* parentScene, ImVec4 backgroundColor, ImVec4 textColor,
		ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize);
	glm::vec2 render() override;

private:

	void hudLives();

	ImVec4 m_hudRed{};
	ImVec4 m_hudWhite{};
	ImVec4 m_hudGreen{};
	ImVec4 m_hudBlue{};

	std::shared_ptr<GameObject> testObject;

};
