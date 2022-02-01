#pragma once
#include "cutScenes/CutScene.h"

#include <memory>

#include "GameObject.h"
#include "Timer.h"

class CutSceneDiscoverTreasure : public CutScene
{

public:
	void run(Scene* scene) override;
	void start() override;
	void update() override;
	void end() override;

private:

	int m_currentAct{ 1 };
	int m_numberOfActs{ 2 };

	GameObject* _player;
	GameObject* _frank;
	Timer scene1Timer{};

	GameObject* _startPlayer();
	GameObject* _startFrank();

	void _endFrank();
	void _endPlayer();


	



};
