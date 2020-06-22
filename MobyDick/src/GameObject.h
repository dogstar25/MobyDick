#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <array>
#include <map>
#include <memory>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include <Box2D/Box2D.h>

#include "TextureManager.h" //Need this here so that inherited classes can get the textureManager without linker issues
#include "GameObjectDefinition.h"
#include "Globals.h"

//Forward declarations
class Animation;
class Texture;

class GameObject
{
private:

	std::string	
		m_currentAnimationState;
	int
		m_mouseState;
	float
		m_angle;
	b2Vec2
		m_position,
		m_size;
	SDL_Color
		m_color;
	bool
		m_removeFromWorld;

	GameObjectDefinition* m_definition;
	std::shared_ptr<Texture> m_texture;
	std::map<std::string, Animation*> m_animations;
	std::array<std::vector<std::shared_ptr<GameObject>>, constants::CHILD_POSITIONS> m_childObjects;

	void init();
	virtual void onMouseHoverRender();
	virtual void onMouseClickRender();
	virtual void onMouseHoldRender();
	virtual void onMouseClickEvent();
	void updateMouseState();
	b2Vec2 calcChildPosition(std::shared_ptr<GameObject>, int, int, int);
	void updateChildObjects();
	void buildChildren();
	void outlineObject(float lineSize);


protected:
	virtual SDL_Rect  getPositionRect();
	b2Vec2 matchParentRotation(SDL_Rect, SDL_Rect, float);
	void renderChildObjects();
	virtual SDL_Rect* getRenderTextureRect();
	virtual SDL_Texture* getRenderTexture();
	virtual SDL_Surface* getRenderSurface();
	void setCurrentAnimationState(std::string animationState) {	m_currentAnimationState = animationState; }
	void setTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

public:
	
	GameObject();
	GameObject(std::string,float,float,float);
	virtual ~GameObject();

	virtual void update();
	virtual void render();

	virtual SDL_Rect  getRenderDestRect();
	virtual void setPosition(b2Vec2 position);
	virtual void setPosition(float xPosition, float yPosition);
	virtual void setPosition(b2Vec2 position, float angle);
	virtual void setPosition(float xPosition, float yPosition, float angle);
	virtual void setSize(b2Vec2 size);
	virtual void setSize(float xSize, float ySize);
	virtual void setAngle(float angle);
	void setColor(SDL_Color color) { 
		m_color = color; 
	}
	void setColorAlpha(int alpha) { 
		m_color.a = alpha; 
	}
	void setColor(int red, int green, int blue, int alpha);
	void setRemoveFromWorld(bool removeFromWorld) { 
		m_removeFromWorld = removeFromWorld; 
	}

	//Accessor Functions
	b2Vec2 size() { 
		return m_size; 
	}
	b2Vec2 position() {	
		return m_position; 
	}
	float angle() { 
		return m_angle; 
	}
	SDL_Color color() { 
		return m_color; 
	}
	std::string currentAnimationState() { 
		return m_currentAnimationState; 
	}
	bool removeFromWorld() { 
		return m_removeFromWorld; 
	}
	Texture* texture() { 
		return m_texture.get(); 
	}

	GameObjectDefinition* definition() { 
		return m_definition; 
	};
	std::map<std::string, Animation*>& animations() { 
		return m_animations; 
	}

	
};

#endif
