#pragma once
#include <json/json.h>
#include "../GameObject.h"
#include "imgui.h"

#include <vector>


class IMGuiItem
{
public:
	IMGuiItem() {};
	IMGuiItem(std::string gameObjectId, b2Vec2 padding, ImVec4 color, bool autoSize);
	
	virtual glm::vec2 render(GameObject* parentGameObject) = 0;
	std::string staticText() { return m_staticText; }

	void setWindowProperties(GameObject* parentGameObject);

protected:

	ImVec4 m_color{};
	bool m_autoSize{};
	b2Vec2 m_padding{};
	ImGuiWindowFlags m_flags{};
	std::string m_gameObjectId{};
	std::string m_staticText{};

	inline static ImFont* m_xsmallFont;
	inline static ImFont* m_smallFont;
	inline static ImFont* m_normalFont;
	inline static ImFont* m_mediumFont;
	inline static ImFont* m_largeFont;

		
};
