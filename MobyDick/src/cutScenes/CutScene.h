#pragma once
#include "../SceneManager.h"
#include <iostream>

enum class cutSceneState {
	IDLE,
	RUNNING
};
class CutScene {

public:
	CutScene() {};
	~CutScene() { std::cout << "delete" << std::endl; };

	virtual void run(Scene* scene) = 0;
	virtual void start() = 0;
	virtual void update() = 0;
	virtual void end() = 0;


protected:
	cutSceneState currentState{ cutSceneState::IDLE };
	Scene* currentScene{};

private:



};

