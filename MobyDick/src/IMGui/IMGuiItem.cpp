#include "IMGuiItem.h"
#include "../game.h"

extern std::unique_ptr<Game> game;

IMGuiItem::IMGuiItem()
{
	game->IMGuiControlled = true;
}

IMGuiItem::~IMGuiItem()
{
	game->IMGuiControlled = false;
}



