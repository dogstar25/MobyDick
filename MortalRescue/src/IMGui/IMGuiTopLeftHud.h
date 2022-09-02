#pragma once
#include "IMGui/IMGuiItem.h"
class IMGuiTopLeftHud : public IMGuiItem
{

public:
	IMGuiTopLeftHud() = default;
	IMGuiTopLeftHud(std::string gameObjectType, b2Vec2 padding, Scene * parentScene, ImVec4 backgroundColor, ImVec4 textColor,
		ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize);
	glm::vec2 render() override;



private:

	void weaponLevel();
	void hudWeaponAccrualBar(ImVec4 color, float accrualValue);
	void hudWeaponChargeBar(float accrualValue);
	void hudWeaponPistolImage(ImVec4 gunColor);


	ImVec4 m_hudRed{};
	ImVec4 m_hudWhite{};
	ImVec4 m_hudGreen{};
	ImVec4 m_hudBlue{};

};

