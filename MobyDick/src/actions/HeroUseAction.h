#ifndef HERO_USE_ACTION_H
#define HERO_USE_ACTION_H

#include "Action.h"

class HeroUseAction : public Action
{
public:

	HeroUseAction();
	~HeroUseAction();

	void perform(Entity entity) override;



};

#endif

