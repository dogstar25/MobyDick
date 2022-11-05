#include "NavigationComponent.h"
#include "../EnumMap.h"
#include "../RayCastCallBack.h"
#include "../game.h"
#include <thread>
#include <glm/glm.hpp>

extern std::unique_ptr<Game> game;

NavigationComponent::~NavigationComponent()
{ 

}

NavigationComponent::NavigationComponent(Json::Value componentJSON)
{
	m_componentType = ComponentTypes::NAVIGATION_COMPONENT;

	m_pathRefreshTimer = {0.5, true};

}


bool NavigationComponent::navigateTo(float pixelX, float pixelY)
{

	m_targetPixelDestination = { pixelX, pixelY };
	m_targetTileDestination =  util::pixelToTileLocation(pixelX, pixelY) ;

	bool pathFound{};

	//If we can "see" the target destination, then move towards it
	//ToDo:Add code to  check if we can see the location. Assuming raycast


	//If we cannot "see" the target destination, then apply the pathfinding algorithm
	//This AStar algorithm can be expensive so only update based on a timer
	if (m_pathRefreshTimer.hasMetTargetDuration() || m_solutionPath.empty() == true) {

		m_solutionPath.clear();
		m_currentNavStep = 0;
		//std::thread t1([&](NavigationComponent* foo) { foo->_findPathToDestination2(); }, this);
		if (m_solutionPath.empty()) {
			pathFound = _buildPathToDestination();
		}

		if (pathFound == false) {
			return false;
		}
	}

	//If we have been stuck in the same spot for a certain period then return true so that A new destination will be determined


	//Have we reached the taregtDestination
	if (util::calculateDistance(parent()->getCenterPosition(), m_targetPixelDestination) < NAV_DISTANCE_TOLERANCE) {
	
		m_solutionPath.clear();
		return true;
	}

	//Have we reached the next interim navigation step
	const auto objectSize = parent()->getSize();
	SDL_Point interimNavStepTileLocation = m_solutionPath.at(m_currentNavStep);
	SDL_FPoint interimNavStepPixelLocation = util::tileToPixelLocation(
		interimNavStepTileLocation.x, interimNavStepTileLocation.y, objectSize.x, objectSize.y);

	//Distance between our master oobject and the destination
	float targetDistance = util::calculateDistance(parent()->getCenterPosition(), interimNavStepPixelLocation);

	if (targetDistance < NAV_DISTANCE_TOLERANCE) {

		if ((m_currentNavStep+1) < m_solutionPath.size()) {
			m_currentNavStep++;
		}

	}

	//Execute Moves to get to targetDestination
	_moveTo(m_solutionPath.at(m_currentNavStep));

	return false;
}



void NavigationComponent::update()
{


}

void NavigationComponent::postInit()
{

}

bool NavigationComponent::_buildPathToDestination()
{
	std::unordered_map<std::string, std::shared_ptr<AStarNode>> toSearch{};
	std::unordered_map<std::string, std::shared_ptr<AStarNode>> processed{};

	//Get current position
	SDL_FPoint currentPosition = parent()->getCenterPosition();

	std::shared_ptr<AStarNode> startingNode = std::make_shared<AStarNode>();

	//Make key
	SDL_Point tileLocation = util::pixelToTileLocation(currentPosition.x, currentPosition.y);
	std::string key = util::locationToString(tileLocation.x, tileLocation.y);

	//set position (in tiles)
	startingNode->position = tileLocation;
	startingNode->keyValue = key;
	toSearch.emplace(std::pair<std::string, std::shared_ptr<AStarNode>>(key, startingNode));

	//While there are nodes left to search
	while (toSearch.empty() == false) {

		//set current to first node
		auto currentItr = toSearch.begin();
		auto currentNode = currentItr->second;

		//Loop through all nodes to find one "better" than the current
		std::unordered_map<std::string, std::shared_ptr<AStarNode>>::iterator i = toSearch.begin();
		while (i != toSearch.end())
		{
			auto nodeCandidate = i->second;
			//If this candidate node "has a lower fcost or "better" than the current node
			if (nodeCandidate->fCost < currentNode->fCost ||
				(nodeCandidate->fCost == currentNode->fCost && nodeCandidate->hCost < currentNode->hCost)) {
				currentNode = nodeCandidate;
				currentItr = i;
			}

			i++;
		}

		//Add the current node to the processed and remove it from the toDo
		processed.emplace(std::pair<std::string, std::shared_ptr<AStarNode>>(currentNode.get()->keyValue, currentNode));
		toSearch.erase(currentItr);

		//If this current node is actually our target node then build the path back and return
		if (currentNode->position.x == m_targetTileDestination.x &&
			currentNode->position.y == m_targetTileDestination.y) {

			//This first node should be the destination node so add it to the solution
			AStarNode* pathNode{ currentNode.get() };
			m_solutionPath.insert(m_solutionPath.begin(), pathNode->position);

			//Now traverse the rest of the nodes using each ones connection key
			while (true) {

				if (pathNode->position.x == startingNode->position.x &&
					pathNode->position.y == startingNode->position.y) {
					break;
				}

				pathNode = processed[pathNode->connectionKey].get();

				//m_solutionPath2.emplace_front(*pathNode);
				m_solutionPath.insert(m_solutionPath.begin(), pathNode->position);
			}

			return true;

		}

		//Get and store all eight neighbors in an array
		// check the navigation map to see which ones are impassable and do not include them
		std::vector<std::shared_ptr<AStarNode>> neighbors{};
		_buildNeighbors(*currentNode, neighbors);

		//For this new current node, loop through all 8 neighbor tiles
		for (auto neighbor : neighbors) {

			//Only process if it hasnt already been processed
			if (_listContainsNode(neighbor.get(), processed) == false) {


				bool inSearchList = _listContainsNode(neighbor.get(), toSearch);

				//Calculate all of this nodes costs
				_calculateCosts(startingNode.get(), neighbor.get());

				//if this node isnt currently in the search list OR
				//this gCost is better than the current one
				if (!inSearchList || (neighbor->gCost < currentNode->gCost)) {

					neighbor->connectionKey = currentNode.get()->keyValue;


					if (!inSearchList) {
						toSearch.emplace(std::pair<std::string, std::shared_ptr<AStarNode>>(neighbor.get()->keyValue, neighbor));
					}

				}

			}

		}

	}

	return false;

}


void NavigationComponent::_calculateCosts(AStarNode* startingNode, AStarNode* node)
{

	float startX = startingNode->position.x;
	float startY = startingNode->position.y;

	float evalNodeX = node->position.x;
	float evalNodeY = node->position.y;

	//Calculate GCost - distance from start
	node->gCost = util::calculateDistance(startingNode->position, node->position);

	//Calculate the HCost - distance from the destination
	node->hCost = util::calculateDistance(node->position, m_targetTileDestination);

	//Calculate the FCost which is the total of the other 2 costs
	node->fCost = node->hCost + node->gCost;

}

bool NavigationComponent::_listContainsNode(AStarNode* node, std::unordered_map<std::string, std::shared_ptr<AStarNode>>& list)
{

	auto search = list.find(node->keyValue);
	if (search != list.end()) {
		return true;
	}

	return false;

}


void NavigationComponent::_buildNeighbors(AStarNode& currentNode, std::vector<std::shared_ptr<AStarNode>>& neighbors)
{
	int x{};
	int y{};

	//TopLeft
	//int x = currentNode.position.x - 1;
	//int y = currentNode.position.y - 1;
	//_addNeighbor(x, y, neighbors);

	//Top
	x = currentNode.position.x;
	y = currentNode.position.y - 1;
	_addNeighbor(x, y, neighbors);

	//TopRight
	//x = currentNode.position.x + 1;
	//y = currentNode.position.y - 1;
	//_addNeighbor(x, y, neighbors);

	//Left
	x = currentNode.position.x - 1;
	y = currentNode.position.y;
	_addNeighbor(x, y, neighbors);

	//Right
	x = currentNode.position.x + 1;
	y = currentNode.position.y;
	_addNeighbor(x, y, neighbors);

	//BottomLeft
	//x = currentNode.position.x - 1;
	//y = currentNode.position.y - 1;
	//_addNeighbor(x, y, neighbors);

	//Bottom
	x = currentNode.position.x;
	y = currentNode.position.y + 1;
	_addNeighbor(x, y, neighbors);

	//BottomRight
	//x = currentNode.position.x + 1;
	//y = currentNode.position.y + 1;
	//_addNeighbor(x, y, neighbors);

}

void NavigationComponent::_addNeighbor(int x, int y, std::vector<std::shared_ptr<AStarNode>>& neighbors)
{

	if (_isValidNode(x, y)) {

		std::shared_ptr<AStarNode> starNode = std::make_shared<AStarNode>();
		starNode->position = { x, y };
		starNode->keyValue = util::locationToString(x,y);
		neighbors.push_back(starNode);

	}

}
bool NavigationComponent::_isValidNode(const int x, const int y) 
{

	const auto& navMap = parent()->parentScene()->navigationMap();
	int xMax = navMap.size();
	int yMax = navMap[0].size();

	//Check map boundaries
	if (x >= 0 && x < xMax && y > 0 && y < yMax) {

		//Check if a passable object
		if (navMap[x][y].passable == true) {

			return true;
		}

	}

	return false;

}

void NavigationComponent::_moveTo(SDL_Point destinationTile)
{

	//Need the objects size to calc the right pixel position
	const auto objectSize = parent()->getSize();

	SDL_FPoint destinationPixelLoc = util::tileToPixelLocation(destinationTile.x, destinationTile.y, objectSize.x, objectSize.y);

	b2Vec2 trajectory{};
	trajectory.x = destinationPixelLoc.x - parent()->getCenterPosition().x;
	trajectory.y = destinationPixelLoc.y - parent()->getCenterPosition().y;

	trajectory.Normalize();

	const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	const auto& moveAction = actionComponent->getAction(ACTION_MOVE);

	moveAction->perform(parent(), trajectory);

	_applyAvoidanceMovement(trajectory);

	moveAction->perform(parent(), trajectory);

	//Set the angle to point towards the next nav point using the trajectory we calculated above
	//_rotateTowards(trajectory, parent());


}


void NavigationComponent::_applyAvoidanceMovement(b2Vec2& trajectory)
{

	const auto& physics = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	//Check all sensor detected objects and see if any are close enough to prompt a movement adjustment
	for (const auto& barrierItem : parent()->getSeenObjects()) {

		//If I see a barrier or a baddie and it within X pixels, then apply an adjustment to the trajectory so that we dont
		//get caught on corners and baddies dont get caught on each othersuch
		//ToDo:need a way to configure which trait tags are used here. Since this is base code, we chouldnt assume that this object should apply 
		// special movement for other baddies - baddies may not even be a thing for this game
		if (barrierItem.gameObject.expired() == false &&
			(barrierItem.gameObject.lock().get()->hasTrait(TraitTag::barrier)
				|| barrierItem.gameObject.lock().get()->hasTrait(TraitTag::baddie))) {

			//Distance
			float distance = util::calculateDistance(parent()->getCenterPosition(), barrierItem.gameObject.lock()->getCenterPosition());


			//Avoid for other baddies
			if (distance < 64 && barrierItem.gameObject.lock().get()->hasTrait(TraitTag::baddie)) {

				//Easy 90 degree applied to trajectory
				b2Vec2 avoidTrajectory90 = { trajectory.y, -trajectory.x };

				//Apply 45 degrees to  trajectory
				b2Vec2 avoidTrajectory45 = { (trajectory.x + trajectory.y) * sqrtf(2), (trajectory.y - trajectory.x) * sqrtf(2) };

				trajectory = avoidTrajectory45;
				break;

			}

			//Avoid a wall if its too close
			//This code below MAY help from getting stuck. I havent proven that. Ima leveave it in for now and revisit when needed
			if (distance < 50 && barrierItem.gameObject.lock().get()->hasTrait(TraitTag::barrier)) {

				//which is the closer to the wall, the X or the Y direction
				float xDistance = abs(parent()->getCenterPosition().x - barrierItem.gameObject.lock()->getCenterPosition().x);
				float yDistance = abs(parent()->getCenterPosition().y - barrierItem.gameObject.lock()->getCenterPosition().y);

				if (xDistance > yDistance) {
					trajectory.x = trajectory.x + -(barrierItem.normal.x);
				}
				else {
					trajectory.y = trajectory.y + -(barrierItem.normal.y);
				}

				break;

			}

		}


	}

}



