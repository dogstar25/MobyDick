#pragma once
#include "Component.h"

class UIControlComponent : public Component
{
public:

	UIControlComponent();
	UIControlComponent(Json::Value definitionJSON);
	~UIControlComponent();
	void update();


private:
	std::bitset<8> m_controls;





	

};

