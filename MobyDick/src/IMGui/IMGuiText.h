#pragma once
#include "../GameObject.h"
#include "IMGuiItem.h"

#include <memory>

class IMGuiText : public IMGuiItem
{
public:
	IMGuiText() = default;
	IMGuiText(std::string gameObjectType, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor, ImVec4 buttonColor, ImVec4 buttonHoverColor,
		ImVec4 buttonActiveColor, bool autoSize, std::string staticTextValue);
	glm::vec2 render() override;

private:

	std::string m_staticTextValue{};

};