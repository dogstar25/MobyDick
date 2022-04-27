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

	std::shared_ptr<GameObject> getInteractionObject() {
		return m_interactionObject;
	}

	void setInteractionObject(std::shared_ptr<GameObject> interactionObject) {
		m_interactionObject = interactionObject;
	}

private:

	std::shared_ptr<IMGuiItem> m_IMGuiItem;
	std::shared_ptr<GameObject> m_interactionObject{};
	
};
