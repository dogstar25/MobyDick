#pragma once
#include <memory>
#include <string>
#include "triggers/TriggerFactory.h"
#include "triggers/Trigger.h"
#include <SDL2/SDL.h>
#include <box2d/box2d.h>

class MRTriggerFactory : public TriggerFactory
{
public:
	MRTriggerFactory();

	std::shared_ptr<Trigger> create(std::string triggerType) override;

private:

};

