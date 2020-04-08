#include "Level.h"
#include <vector>


void Level::addWaypoint(Waypoint wayPoint)
{

	m_patrolWaypoints.push_back(wayPoint);


}

void Level::addLevelObject(int xIndex, int yIndex, LevelObject levelObject)
{

	levelObjects[xIndex][yIndex]=levelObject;

}

void Level::setLevelObjectArraySize(int width, int height)
{

	levelObjects.resize(width, std::vector<LevelObject>(height));

}
