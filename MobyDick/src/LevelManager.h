#pragma once

#include <string>
#include <vector>
#include <optional>
#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <json/json.h>
#include <map>

#include "GameObject.h"
#include "Globals.h"
#include "BaseConstants.h"
#include "triggers/Trigger.h"


class Scene;

struct LevelObject
{
	std::string gameObjectId {};
	int layer{ LAYER_MAIN };
	int angleAdjustment {};
	bool cameraFollow{ false };
};

class LevelManager
{
public:

	static LevelManager& instance();

	std::string m_id; //probably same as the textureId since the map is represented by a texture
	int m_width, m_height; // in tile count
	int m_tileWidth, m_tileHeight;
	SDL_Rect m_levelBounds;

	void update(Scene* scene);
	void load(std::string levelId, Scene* scene);
	void clearTriggers();

	void addLevelObject(int xIndex, int yIndex, LevelObject levelObject);
	void setLevelObjectArraySize(int width, int height);

	//Accessor Functions
	std::string description() {	return m_description; }
	//const std::vector<Waypoint>& waypoints() { return m_waypoints; }

private:

	LevelManager();
	~LevelManager();

	std::string m_description;
	std::string m_blueprintTexture;
	Json::Value m_locationList;
	std::vector<std::shared_ptr<Trigger>> m_levelTriggers;

	std::vector< std::vector <LevelObject>> m_levelObjects;

	std::optional<LevelObject> _determineTile(int x, int y, SDL_Surface* bluePrintSurface);
	LevelObject _determineWallObject(int x, int y, SDL_Surface* bluePrintSurface);
	std::optional<LevelObject> _determineLocationObject(int x, int y, SDL_Surface* bluePrintSurface);
	void _loadDefinition(std::string levelId);
	void _buildLevelObjects(Scene* scene);

};

