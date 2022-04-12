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

	virtual std::shared_ptr<IMGuiItem> create(
		std::string iMGuiItemType, 
		std::string gameObjectId, 
		b2Vec2 padding, 
		ImVec4 backgroundColor, 
		ImVec4 textColor,
		ImVec4 buttonColor, 
		ImVec4 buttonHoverColor, 
		ImVec4 buttonActiveColor, 
		bool autoSize, 
		std::string staticTextValue);

private:

};
