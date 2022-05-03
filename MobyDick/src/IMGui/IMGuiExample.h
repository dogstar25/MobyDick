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
	glm::vec2 render() override;

private:
	bool show_another_window = true;

};