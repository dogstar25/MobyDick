#pragma once
#include "Component.h"
#include <json/json.h>
#include <memory>

class IMGuiItem;

class IMGuiComponent : public Component {


public:
	IMGuiComponent() = default;
	IMGuiComponent(Json::Value componentJSON, std::string gameObjectId);

	void update() override;
	void render();

	std::shared_ptr<IMGuiItem> getIMGuiItem() { return m_IMGuiItem; }
	void setParent(GameObject* parentObject);

private:

	std::shared_ptr<IMGuiItem> m_IMGuiItem;
	
};
