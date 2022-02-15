#pragma once
#include "Component.h"
#include <json/json.h>

#include "../IMGui/IMGuiItem.h"

class IMGuiComponent : public Component {


public:
	IMGuiComponent() {};
	IMGuiComponent(Json::Value componentJSON);

	void update() override;
	void render();

private:

	std::shared_ptr<IMGuiItem>m_IMGuiItem{};
	
};
