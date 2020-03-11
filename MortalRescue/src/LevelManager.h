#pragma once
#include "Level.h"

#include <SDL.h>
#include <Box2D/Box2D.h>
#include <vector>
#include <map>
#include <string>


class LevelManager
{
public:

	std::map<std::string, Level*> levels;

	void loadLevelBlueprint(std::string);
	void buildLevel(std::string);

private:
	LevelObject* determineTile(int, int, SDL_Surface*);


};

