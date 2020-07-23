#include "PlayerControlComponent.h"
#include "../EnumMaps.h"

PlayerControlComponent::PlayerControlComponent(Json::Value definitionJSON)
{

	Json::Value componentJSON = definitionJSON["playerControlComponent"];

	for (Json::Value itrControls : componentJSON["controls"])
	{
		int controlFlag = EnumMap::instance().toEnum(itrControls.asString());
		m_controls.set(controlFlag);


	}


}

