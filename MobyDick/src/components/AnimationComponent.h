#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <memory>

#include <string>
#include <map>
#include <chrono>
#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <json/json.h>
#include "../texture.h"

struct Animation
{

	float m_speed;
	int	m_state;
	int m_frameCount;
	int m_currentAnimFrame;
	b2Vec2 m_frameSize;
	std::chrono::steady_clock::time_point m_timeSnapshot;

	//a rectangle pointing to the animation textture of the animation frame to be displayed
	std::shared_ptr<SDL_Rect> m_currentTextureAnimationSrcRect;

	//Array of all x,y coordinates of the top left corner of each animation frame in the texture
	std::vector<SDL_FPoint> m_animationFramePositions;

	std::shared_ptr<Texture> m_texture;
	void animate();

	Animation();
	Animation(Json::Value animationDetailsJSON, Json::Value transformComponentJSON);

};

struct AnimationComponent
{

	int m_currentAnimationState;
	std::map<int, std::shared_ptr<Animation>> m_animations;

	AnimationComponent();
	AnimationComponent(Json::Value definitionJSON);

	SDL_Rect* getCurrentAnimationTextureRect();
	SDL_Texture* getCurrentAnimationTexture();

};

#endif