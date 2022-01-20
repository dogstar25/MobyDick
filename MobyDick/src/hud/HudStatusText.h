#pragma once
#include "HudItem.h"

#include "../GameObject.h"

#include <memory>
#include <optional>
#include <string>

class HudStatusText : public HudItem
{

public:
	HudStatusText() = default;
	HudStatusText(std::string labelObjectId, std::string statusObjectId, std::string statusValueId, float labelPadding, Scene* parentScene);
	void update(GameObject* parentGameObject) override;
	void render(GameObject* parentGameObject) override;

private:


};

