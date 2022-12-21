#pragma once
#include "IMGui/IMGuiItem.h"
class IMGuiTopRightHud : public IMGuiItem
{

public:
	IMGuiTopRightHud() = default;
	IMGuiTopRightHud(std::string gameObjectType, b2Vec2 padding, Scene * parentScene, ImVec4 backgroundColor, ImVec4 textColor,
		ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize);
	glm::vec2 render() override;



private:

	void survivorCount();
	void hudSurvivorImage(ImVec4 color);
	void droneCount();
	void hudDroneImage(ImVec4 color);
	void _adjustForVariableStatusItems();

	ImVec4 m_hudRed{};
	ImVec4 m_hudWhite{};
	ImVec4 m_hudGreen{};
	ImVec4 m_hudBlue{};
	ImVec4 m_hudYellow{};
	ImVec4 m_hudOrange{};

	bool m_hasSurvivorDisplay{};
	bool m_hasDroneDisplay{};

};

