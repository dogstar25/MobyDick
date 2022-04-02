#pragma once
#include "../GameObject.h"
#include "IMGuiItem.h"

#include <memory>

class IMGuiText : public IMGuiItem
{
public:
	IMGuiText() = default;
	IMGuiText(std::string gameObjectId, b2Vec2 padding, ImVec4 color, bool autoSize, std::string staticTextValue);
	glm::vec2 render(GameObject* parentGameObject) override;

private:

	std::string m_staticTextValue{};

};