#pragma once
#include <string>
#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <assert.h>

class GameObject;

class Action
{
public:

	Action() {}
	~Action();

	//We never want to end up calling the base level action perform. The derived action classes should have setup
	//the proper perform override and whoever is calling these actions should be passing in the correct paramters
	//If we end up calling base, then throw an assert
	virtual void perform() { assert(false && "An Empty Action was executed!"); };
	virtual void perform(GameObject*) { assert(false && "An Empty Action was executed!"); };
	virtual void perform(GameObject*, int) { assert(false && "An Empty Action was executed!"); };
	virtual void perform(GameObject*, int, int) { assert(false && "An Empty Action was executed!"); };
	virtual void perform(GameObject*, b2Vec2) { assert(false && "An Empty Action was executed!"); };
	virtual void perform(GameObject*, float) { assert(false && "An Empty Action was executed!"); };
	virtual void perform(SDL_FRect*, glm::vec2, float) { assert(false && "An Empty Action was executed!"); };
	virtual void perform(GameObject*, SDL_Scancode) { assert(false && "An Empty Action was executed!"); };
	virtual void perform(GameObject*, GameObject*, SDL_Scancode) { assert(false && "An Empty Action was executed!"); };
	
};

