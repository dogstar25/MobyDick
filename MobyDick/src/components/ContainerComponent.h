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
	ContainerComponent(Json::Value componentJSON, Scene* parentScene);
	~ContainerComponent();

	void update() override;
	void render();
	void postInit() override;

	void addItem(std::string gameObjectId, float spawnForce, Scene* parentScene, bool onContainerConstruction=false);
	bool isEmpty();
	bool isFull();
	std::vector<ContainerItem>& items() { return m_items; }

private:

	int m_capacity{};
	bool m_doesAutoRefill{};
	Timer m_refillTimer{};
	std::string m_contentItemGameObjectId{};
	float m_contentsItemSpawnForce{};


	std::vector<ContainerItem> m_items{};

	void _removeFromWorldPass();
	void _setPieceLocationAndForce(const std::shared_ptr<TransformComponent> containerTransform, ContainerItem containerItem);

};