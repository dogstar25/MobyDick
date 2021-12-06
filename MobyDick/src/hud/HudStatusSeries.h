#pragma once
#include "HudItem.h"
#include "../GameObject.h"

#include <memory>
#include <optional>
#include <string>

class HudStatusSeries :  public HudItem
{

public:
	HudStatusSeries() = default;
	HudStatusSeries(std::string labelObjectId, std::string statusObjectId, float labelPadding, Scene * parentScene);
	void update(GameObject * parentGameObject) override;
	void render(GameObject * parentGameObject) override;

private:

};

