#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <map>

#include "Waypoint.h"

struct LevelObject
{
	std::string entityDefinitionId;
	int angleAdjustment;
};

class Level
{
public:

	std::string m_id; //probably same as the textureId since the map is represented by a texture
	int m_width, m_height; // in tile count
	float m_tileWidth, m_tileHeight;
	SDL_Rect m_levelBounds;

	std::vector< std::vector <LevelObject> > levelObjects;

	static Level& instance();
	void addWaypoint(Waypoint waypoint);
	void addLevelObject(int xIndex, int yIndex, LevelObject levelObject);
	void setLevelObjectArraySize(int width, int height);

	void load(std::string levelId);

	//Accessor Functions
	std::string description() {
		return m_description;
	}
	const std::vector<Waypoint>& waypoints() {
		return m_waypoints;
	}

private:
	std::string m_description;
	std::string m_blueprint;

	std::vector<Waypoint> m_waypoints;
	std::map<std::string, LevelObject*> m_locationObjects;

	LevelObject* _determineTile(int, int, SDL_Surface*);
	void _loadDefinition(std::string levelId);
	void _buildLevelObjects();

};


#endif