#pragma once
#include "Component.h"
#include "../GameObject.h"

#include <json/json.h>
#include <list>
#include <memory>


inline constexpr int NAV_DISTANCE_TOLERANCE = 64;

struct AStarNode
{
	std::optional<std::shared_ptr<AStarNode>> connection{};
	SDL_Point position{};
	float gCost{};
	float hCost{};
	float fCost{};

	std::string connectionKey{};
	std::string keyValue{};

};

class NavigationComponent : public Component
{

public:
	NavigationComponent() = default;
	NavigationComponent(Json::Value componentJSON);
	~NavigationComponent();

	void update() override;
	void postInit() override;
	bool navigateTo(float pixelX, float pixelY);
	void navigateStop();


private:

	std::vector<SDL_Point>m_solutionPath{};

	bool _buildPathToDestination();
	void _buildNeighbors(AStarNode& currentNode, std::vector<std::shared_ptr<AStarNode>>& neighbors);
	bool _isValidNode(const int x, const int y);
	void _addNeighbor(int x, int y, std::vector<std::shared_ptr<AStarNode>>& neighbors);
	bool _listContainsNode(AStarNode* node, std::unordered_map<std::string, std::shared_ptr<AStarNode>>& list);
	void _calculateCosts(AStarNode* startingNode, AStarNode* node);
	void _moveTo(SDL_Point destinationTile);
	void _applyAvoidanceMovement(b2Vec2& trajectory);
	void _applyAvoidanceMovement2(b2Vec2& trajectory);


	Timer m_pathRefreshTimer{ 0.5, true };
	SDL_FPoint m_targetPixelDestination{};
	SDL_Point m_targetTileDestination{};
	int m_currentNavStep{};





};

