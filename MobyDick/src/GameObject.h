#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include <Box2D/Box2D.h>

#include "Component.h"
#include "GameObjectDefinition.h"
#include "Globals.h"

//Forward declarations
class Animation;
class Texture;

class GameObject
{
public:
	
	GameObject();
	GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust);
	~GameObject();

	virtual void update();
	virtual void render();

	void setRemoveFromWorld(bool removeFromWorld) { 
		m_removeFromWorld = removeFromWorld; 
	}

	//Accessor Functions
	bool removeFromWorld() { 
		return m_removeFromWorld; 
	}

	std::shared_ptr<GameObjectDefinition> gameObjectDefinition(){
		return m_gameObjectDefinition;
	}


	
private:

	int
		m_mouseState;
	bool
		m_removeFromWorld;
	std::shared_ptr<GameObjectDefinition> m_gameObjectDefinition;

	void init();
	/*virtual void onMouseHoverRender();
	virtual void onMouseClickRender();
	virtual void onMouseHoldRender();
	virtual void onMouseClickEvent();
	void updateMouseState();*/

	std::vector<Component> m_components;


};

#endif
