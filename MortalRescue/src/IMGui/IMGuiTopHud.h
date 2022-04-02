#pragma once

#include "IMGui/IMGuiItem.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "GameObject.h"
#include <memory>


class IMGuiTopHud : public IMGuiItem{

public:
	IMGuiTopHud() = default;
	IMGuiTopHud(std::string gameObjectId, b2Vec2 padding, ImVec4 color, bool autoSize);
	glm::vec2 render(GameObject* parentGameObject) override;

private:

	void hudScrapCount();
	void hudScrapBar();

};
