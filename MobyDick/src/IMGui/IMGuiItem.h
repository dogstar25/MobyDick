#pragma once
#include <json/json.h>
#include "../GameObject.h"
#include "imgui.h"

#include <vector>


class IMGuiItem
{
public:
	IMGuiItem() {};
	IMGuiItem(std::string gameObjectType, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor, ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize);
	//IMGuiItem(std::string gameObjectId, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor, bool autoSize);
	
	virtual glm::vec2 render() = 0;
	std::string staticText() { return m_staticText; }

	void setWindowProperties(GameObject* parentGameObject);
	GameObject* parent() { return m_parent; }
	void setParent(GameObject* gameObject) { m_parent = gameObject; }

protected:

	ImVec4 m_backgroundColor;
	ImVec4 m_textColor; 
	ImVec4 m_buttonColor; 
	ImVec4 m_buttonHoverColor; 
	ImVec4 m_buttonActiveColor;

	bool m_autoSize{};
	b2Vec2 m_padding{};
	ImGuiWindowFlags m_flags{};
	std::string m_gameObjectType{};
	std::string m_staticText{};

	inline static ImFont* m_xSmallFont;
	inline static ImFont* m_smallFont;
	inline static ImFont* m_normalFont;
	inline static ImFont* m_mediumFont;
	inline static ImFont* m_largeFont;
	inline static ImFont* m_xLargeFont;

	//GameObject that owns this IMGuiItem
	GameObject* m_parent{};

		
};
