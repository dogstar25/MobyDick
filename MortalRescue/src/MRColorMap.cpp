#include "MRColorMap.h"
#include "GameConstants.h"

MRColorMap::MRColorMap()
{

	addColor("Colors::NOCOLOR", Colors::NOCOLOR);

	//greens
	addColor("Colors::FOREST", Colors::FOREST);
	addColor("Colors::EMERALD", Colors::EMERALD);
	addColor("Colors::PARAKEET", Colors::PARAKEET);

	//reds
	addColor("Colors::WINE", Colors::WINE);
	addColor("Colors::GRAPE", Colors::GRAPE);

	//yellows
	addColor("Colors::GOLD", Colors::GOLD);

	//blues
	addColor("Colors::CYAN", Colors::CYAN);
	addColor("Colors::MARINE", Colors::MARINE);
	addColor("Colors::SAPPHIRE", Colors::SAPPHIRE);

	//Dark
	addColor("Colors::CHARCOAL", Colors::CHARCOAL);
	addColor("Colors::MIRKWOOD", Colors::MIRKWOOD);
	addColor("Colors::SLATE", Colors::SLATE);

}