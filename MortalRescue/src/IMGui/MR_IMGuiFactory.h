#pragma once
#include <memory>
#include <string>
#include "IMGui/IMGuiItem.h"
#include "IMGui/IMGuiFactory.h"
#include <SDL2/SDL.h>
#include <box2d/box2d.h>

class MR_IMGuiFactory : public IMGuiFactory
{
public:
	MR_IMGuiFactory();

	std::shared_ptr<IMGuiItem> create(std::string iMGuiItemType, std::string gameObjectId, b2Vec2 padding, ImVec4 color, bool autoSize, std::string staticTextValue) override;

private:

};
