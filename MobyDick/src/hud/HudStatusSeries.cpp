#include "HudStatusSeries.h"
#include "../game.h"

#include <algorithm>

extern std::unique_ptr<Game> game;

HudStatusSeries::HudStatusSeries(std::string labelObjectId, std::string statusObjectId, float labelPadding, Scene* parentScene)
{

	m_label = std::make_shared<GameObject>(labelObjectId, -50.0f, -50.0f, 0.0f, parentScene, LAYER_MAIN);
	const auto& labelTransformComponent = m_label.value()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	labelTransformComponent->setAbsolutePositioning(true);

	m_statusObject = std::make_shared<GameObject>(statusObjectId, -50.0f, -50.0f, 0.0f, parentScene, LAYER_MAIN);
	const auto& statusTransformComponent = m_statusObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	statusTransformComponent->setAbsolutePositioning(true);

	m_labelPadding = labelPadding;
	m_statusValue = game->statusMananger()->hudValueMap()[statusObjectId].get();


}

void HudStatusSeries::update(GameObject* parentGameObject)
{

	float labelPositionAdjustment{};
	float labelHeight{};
	float labelWidth{};

	const auto& statusTransformComponent = m_statusObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	auto statusObjectWidth = statusTransformComponent->getPositionRect().w;
	auto statusObjectHeight = statusTransformComponent->getPositionRect().h;

	//If we have a lable object then update its position based on its parent and 
	//also call its normal update 
	if (m_label.has_value()) {

		SDL_FPoint labelPosition;
		m_label.value()->update();

		const auto& labelTransformComponent = m_label.value()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
		labelWidth = labelTransformComponent->getPositionRect().w;

		labelPositionAdjustment = (labelWidth / 2) + (statusObjectWidth / 2) + m_labelPadding;
		labelHeight = labelTransformComponent->getPositionRect().h;

		float height = std::max(labelHeight, statusObjectHeight);

		labelPosition.x = parentGameObject->getTopLeftPosition().x + (labelWidth / 2);
		labelPosition.y = parentGameObject->getTopLeftPosition().y + (height / 2);
		m_label.value()->setPosition(labelPosition);
	}

	//Update the statusObject
	m_statusObject->update();

	//Update the position of the first status Object
	auto parentPosition = parentGameObject->getCenterPosition();

	SDL_FPoint statusPosition = parentPosition;
	statusPosition.x = parentPosition.x + labelPositionAdjustment;
	m_statusObject->setPosition(statusPosition);

	//Calculate the size of the hud item
	float hudHeight{};
	float hudWidth{};

	//Width - Label size plus the status object repeated
	hudWidth = labelWidth + (statusObjectWidth * (*m_statusValue));

	//Height
	hudHeight = std::max(labelHeight, statusObjectHeight);

	const auto& parentTransformComponent = parentGameObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	parentTransformComponent->setSize(hudWidth, hudHeight);


}

void HudStatusSeries::render(GameObject* parentGameObject)
{
	const auto& statusTransformComponent = m_statusObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	float x{};
	float y{};


	if (m_label.has_value()) {
		m_label.value()->render();
	}

	m_statusObject->render();

	//Draw it again
	x = statusTransformComponent->getCenterPosition().x + statusTransformComponent->getPositionRect().w;
	y = statusTransformComponent->getCenterPosition().y;

	m_statusObject->render(x,y);
	//m_statusObject->render(x, y);
	//m_statusObject->render(x, y);


}