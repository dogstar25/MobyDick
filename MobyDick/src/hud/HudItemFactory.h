#pragma once
#include <memory>
#include <string>


class Scene;
class HudItem;

enum class HudItemTypes {
	STATUS_SINGLE,
	STATUS_SERIES,

	LAST_INDEX,
	COUNT
};

class HudItemFactory
{

public:

	static HudItemFactory& instance();
	std::shared_ptr<HudItem> create(HudItemTypes type, std::string labelId, std::string statusId, float labelPadding, Scene* parentScene);


private:

};

