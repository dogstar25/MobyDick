#pragma once
#include <memory>
#include <string>
#include <json/json.h>
#include <box2d/box2d.h>
#include <SDL2/SDL.h>
#include "imgui.h"

class IMGuiItem;

class IMGuiFactory
{
public:
	IMGuiFactory() = default;

	virtual std::shared_ptr<IMGuiItem> create(std::string iMGuiItemType, std::string gameObjectName, b2Vec2 padding, ImVec4 color, bool autoSize, std::string staticTextValue);

private:

};
