#pragma once
#include <memory>
#include <string>
#include <json/json.h>
#include <box2d/box2d.h>
#include <SDL2/SDL.h>
#include "Trigger.h"

class IMGuiItem;
class Scene;

class TriggerFactory
{
public:
	TriggerFactory() = default;

	virtual std::shared_ptr<Trigger> create(std::string triggerType);

private:

};

