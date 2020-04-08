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

	void loadLevelBlueprint(std::string);
	void buildLevel(std::string);

	b2Vec2 levelSize(std::string level);
	b2Vec2 tileSize(std::string level);

private:
	LevelObject* determineTile(int, int, SDL_Surface*);
	std::map<std::string, Level*> m_levels;

};

