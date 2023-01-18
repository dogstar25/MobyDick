#pragma once
#include <memory>
#include <string>
#include "triggers/TriggerFactory.h"
#include "triggers/Trigger.h"
#include <SDL2/SDL.h>
#include <box2d/box2d.h>

class BBTriggerFactory : public TriggerFactory
{
public:
	BBTriggerFactory();

	std::shared_ptr<Trigger> create(std::string triggerType) override;

private:

};

