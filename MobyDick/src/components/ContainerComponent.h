#pragma once

#include <vector>
#include <string>
#include <chrono>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <json/json.h>

#include "Component.h"
#include "../GameObject.h"
#include "../Scene.h"

#include "TransformComponent.h"

struct ContainerItem {

	float spawnForce{};
	std::shared_ptr<GameObject> gameObject{};

};

class ContainerComponent : public Component
{

public:
	ContainerComponent();
	ContainerComponent(Json::Value componentJSON, std::string parentName, Scene* parentScene);
	~ContainerComponent();

	void update() override;
	void render();
	void postInit() override;

	ContainerItem addItem(std::string gameObjectType, float spawnForce, Scene* parentScene, std::string parentName, int itemCount, bool onContainerConstruction=false);
	void setRefillTimer(float refillTime);
	void setCapacity(int capacity) { m_capacity = capacity; }
	void setStartCount(int startCount) {
		m_startCount = startCount;
	}


	bool isEmpty();
	bool isFull();
	std::vector<ContainerItem>& items() { return m_items; }

private:

	int m_capacity{};
	bool m_doesAutoRefill{};
	Timer m_refillTimer{};
	std::string m_contentItemGameObjectType{};
	float m_contentsItemSpawnForce{};
	int m_startCount{};
	std::string _buildItemName(std::string parentName, int itemCount);


	std::vector<ContainerItem> m_items{};

	void _removeFromWorldPass();
	void _setPieceLocationAndForce(const std::shared_ptr<TransformComponent> containerTransform, ContainerItem containerItem);

};