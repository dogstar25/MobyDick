#pragma once
#include "../GameObject.h"
#include "IMGuiItem.h"

#include <memory>

class IMGuiText : public IMGuiItem
{
public:
	IMGuiText() = default;
	IMGuiText(Json::Value params, std::string windowName);
	glm::vec2 render(SDL_FRect destRect) override;

private:

	std::string m_label;

};