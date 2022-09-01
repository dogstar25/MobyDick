#pragma once
#include <memory>
#include <string>
#include "IMGui/IMGuiItem.h"
#include "IMGui/IMGuiFactory.h"
#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include "Scene.h"

class MR_IMGuiFactory : public IMGuiFactory
{
public:
	MR_IMGuiFactory();

	std::shared_ptr<IMGuiItem> create(std::string iMGuiItemType, std::string gameObjectType, Scene* parentScene, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
		ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize, std::string staticTextValue) override;

private:

};
