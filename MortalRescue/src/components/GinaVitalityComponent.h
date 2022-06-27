#pragma once
#include <json/json.h>
#include "components/VitalityComponent.h"

class GinaVitalityComponent : public VitalityComponent
{
public:
	GinaVitalityComponent();
	GinaVitalityComponent(Json::Value componentJSON);
	~GinaVitalityComponent();

	void update() override;
};
