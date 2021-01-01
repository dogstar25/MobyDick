#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <optional>
#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <json/json.h>
#include <map>

#include "Globals.h"


class Scene;

struct LevelObject
{
	std::string gameObjectId {};
	int type{ LevelLocItemType::GAMEOBJECT };
	int angleAdjustment {};
	int waypoint {};
};

struct Waypoint
{
	int order;
	b2Vec2 point;

	Waypoint(int order, b2Vec2 position) :
		order(order),
		point(position) {};
};

class Level
{
public:

	std::string m_id; //probably same as the textureId since the map is represented by a texture
	int m_width, m_height; // in tile count
	int m_tileWidth, m_tileHeight;
	SDL_Rect m_levelBounds;

	

	static Level& instance();
	void addWaypoint(Waypoint waypoint);
	void addLevelObject(int xIndex, int yIndex, LevelObject levelObject);
	void setLevelObjectArraySize(int width, int height);

	void load(std::string levelId, Scene* scene);

	//Accessor Functions
	std::string description() {	return m_description; }
	const std::vector<Waypoint>& waypoints() { return m_waypoints; }

private:

	Level();
	~Level();

	std::string m_description;
	std::string m_blueprint;
	Json::Value m_locationList;

	std::vector<Waypoint> m_waypoints;
	std::vector< std::vector <LevelObject> > m_levelObjects;
//	std::map<std::string, LevelObject*> m_locationObjects;

	std::optional<LevelObject> _determineTile(int x, int y, SDL_Surface* bluePrintSurface);
	LevelObject _determineWallObject(int x, int y, SDL_Surface* bluePrintSurface);
	std::optional<LevelObject> _determineLocationObject(int x, int y, SDL_Surface* bluePrintSurface);
	void _loadDefinition(std::string levelId);
	void _buildLevelObjects(Scene* scene);

};


#endif