#include "GameObject.h"
#include "game.h"
#include "Weapon.h"


void GameObject::update()
{
	//If this object is animated, then animate it
	if(this->definition->isAnimated) {
		this->definition->animations[this->currentAnimationState]->animate(this);
	}


}

void GameObject::render()
{

	SDL_Rect srcRect, destRect;

	//calculate the destination rectangle - must convert meters to pixels with scale factor
	destRect.w = (this->definition->xSize * Game::config.scaleFactor);
	destRect.h = (this->definition->ySize * Game::config.scaleFactor);
	destRect.x = round((this->physicsBody->GetPosition().x *  Game::config.scaleFactor) - (destRect.w / 2));
	destRect.y = round((this->physicsBody->GetPosition().y *  Game::config.scaleFactor) - (destRect.h / 2));


	float angle = this->physicsBody->GetAngle();

	angle = angle * 180 / M_PI;
	angle = angle += this->angleAdjustment;
	//this->angle = angle;

	//Adjust position based on current camera position - offset
	destRect.x -= Game::camera.frame.x;
	destRect.y -= Game::camera.frame.y;

	//If this is a primitive shape object just drawa a rectangle
	if (this->definition->isPrimitiveShape == true)
	{
		SDL_SetRenderDrawColor(pRenderer,
			this->definition->primativeColor.r,
			this->definition->primativeColor.g,
			this->definition->primativeColor.b,
			this->definition->primativeColor.a);
		SDL_RenderFillRect(pRenderer, &destRect);
		//drawPoly(gameObject->physicsBody);
	}
	else
	{
		//If this is animated object then get its current animation frame texture, 
		// otherwise get its static texture
		SDL_Texture* texure = NULL;
		SDL_Rect *textureSourceRect = NULL;
		if (this->definition->isAnimated) {

			texure = this->definition->animations[this->currentAnimationState]->texture;
			textureSourceRect = &this->definition->animations[this->currentAnimationState]->currentTextureAnimationSrcRect;
		}
		else {

			texure = this->staticTexture;
		}

		//Render th the page
		SDL_RenderCopyEx(pRenderer, texure, textureSourceRect, &destRect, angle,
			NULL, SDL_FLIP_NONE);
	}


}

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
}

GameObjectDefinition::GameObjectDefinition()
{
}

GameObjectDefinition::~GameObjectDefinition()
{
	
	for (auto animation : this->animations)
	{
		delete animation.second;
	}

	this->animations.clear();
	
}





