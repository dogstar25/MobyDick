#pragma once
#include "GameObject.h"
#include "IMGui/IMGuiItem.h"

#include <memory>

class IMGuiInteractiveMenuDoor : public IMGuiItem
{
public:
	IMGuiInteractiveMenuDoor() = default;
	IMGuiInteractiveMenuDoor(std::string gameObjectType, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
		ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize);
	glm::vec2 render() override;

private:

	std::string m_label;

};