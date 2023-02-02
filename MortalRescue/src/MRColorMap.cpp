#include "MRColorMap.h"
#include "GameConstants.h"

MRColorMap::MRColorMap()
{

	addColor("Colors::NOCOLOR", Colors::NOCOLOR);
	addColor("Colors::EGGSHELL", Colors::EGGSHELL);
	
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
	addColor("Colors::TEAL", Colors::TEAL);
	addColor("Colors::ROBINEGG", Colors::ROBINEGG);
	
	//Dark
	addColor("Colors::CHARCOAL", Colors::CHARCOAL);
	addColor("Colors::MIRKWOOD", Colors::MIRKWOOD);
	addColor("Colors::RICK_BLACK", Colors::RICK_BLACK);
	addColor("Colors::SLATE", Colors::SLATE);

}