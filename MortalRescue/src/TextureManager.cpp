#include <iostream>
#include <fstream>
#include <math.h>
#include <list>

#include "TextureManager.h"
#include "game.h"



bool TextureManager::init(SDL_Window* pWindow)
{

	//Create the main renderer
	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 0);

	//SDL_RenderSetScale(pRenderer, (1/Game::config.scaleFactor), (1/Game::config.scaleFactor));
	//SDL_RenderSetScale(pRenderer, 2 ,2);

	//Load all of the textures for the game
	loadTextures();

	return true;
}

bool TextureManager::render(GameObject* gameObject)
{
	SDL_Rect srcRect, destRect;

	//calculate the destination rectangle - must convert meters to pixels with scale factor
	destRect.w = (gameObject->definition->xSize * Game::config.scaleFactor);
	destRect.h = (gameObject->definition->ySize * Game::config.scaleFactor);
	destRect.x = round((gameObject->physicsBody->GetPosition().x *  Game::config.scaleFactor) - (destRect.w /2)) ;
	destRect.y = round((gameObject->physicsBody->GetPosition().y *  Game::config.scaleFactor) - (destRect.h /2)) ;

	
	float angle = gameObject->physicsBody->GetAngle();

	angle = angle * 180 / M_PI;
	angle = angle += gameObject->angleAdjustment;
	//this->angle = angle;

	//Adjust position based on current camera position - offset
	destRect.x -= Game::camera.frame.x;
	destRect.y -= Game::camera.frame.y;

	//If this is a primitive shape object just drawa a rectangle
	if (gameObject->definition->isPrimitiveShape == true)
	{
		SDL_SetRenderDrawColor(pRenderer, 
			gameObject->definition->primativeColor.r,
			gameObject->definition->primativeColor.g,
			gameObject->definition->primativeColor.b,
			gameObject->definition->primativeColor.a);
		SDL_RenderFillRect(pRenderer, &destRect);
		//drawPoly(gameObject->physicsBody);
	}
	else
	{
		//If this is animated object then get its current animation frame texture, 
		// otherwise get its static texture
		SDL_Texture* texure=NULL;
		SDL_Rect *textureSourceRect = NULL;
		if (gameObject->definition->isAnimated) {

			texure = gameObject->definition->animations[gameObject->currentAnimationState]->texture;
			textureSourceRect = &gameObject->definition->animations[gameObject->currentAnimationState]->currentTextureAnimationSrcRect;
		}
		else {

			texure = gameObject->staticTexture;
		}

		//Render th the page
		SDL_RenderCopyEx(pRenderer, texure, textureSourceRect, &destRect, angle,
			NULL, SDL_FLIP_NONE);

	}

	//std::cout << "Dest X is " << destRect.x << " \n";

	return true;
}

bool TextureManager::render(b2Body* body)
{
	SDL_Rect srcRect, destRect;

	//SDL_RenderClear(pRenderer);
	SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 0);
	destRect.x = round(body->GetPosition().x);
	destRect.y = round(body->GetPosition().y);
	destRect.w = 5;
	destRect.h = 5;
	SDL_RenderFillRect(pRenderer, &destRect);
	//SDL_RenderPresent(pRenderer);
	//SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);

	return true;
}

bool TextureManager::loadTextures()
{

	//Read file and stream it to a JSON object
	Json::Value root;
	ifstream ifs("assets/textureAssets.json");
	ifs >> root;

	//Get and store config values
	string filename, id;
	bool retainSurface = false;
	SDL_Surface* surface;
	SDL_Texture* texture;
	Texture* textureObject;


	//Loop through every texture defined in the config file, create a texture object
	//and store it in the main texture map
	for(auto itr : root["textures"])
	{

		textureObject = new Texture();

		id = itr["id"].asString();
		filename = itr["filename"].asString();
		retainSurface = itr["retainSurface"].asBool();

		surface = IMG_Load(filename.c_str());
		texture = SDL_CreateTextureFromSurface(this->pRenderer, surface);

		textureObject->texture = texture;
		if (retainSurface == true)
		{
			textureObject->surface = surface;
		}
		else
		{
			SDL_FreeSurface(surface);
		}
		
		this->textureMap[id]= textureObject;

	}

	return true;
}

Texture * TextureManager::getTexture(string id)
{
	Texture* textureObject;

	textureObject = this->textureMap[id];
	if (textureObject == NULL)
	{
		textureObject = this->textureMap["TX_DEFAULT"];
	}

	return textureObject;
}

bool TextureManager::present()
{
	SDL_RenderPresent(pRenderer);
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 0);

	return true;
}

bool TextureManager::clear()
{
	SDL_RenderClear(pRenderer);

	return true;
}

void TextureManager::drawPoly(b2Body* body)
{


	b2Fixture* fixture = body->GetFixtureList();
	while (fixture != NULL)
	{
		
		b2Shape* s = fixture->GetShape();
		b2PolygonShape* shape = (b2PolygonShape*)s;

		SDL_Point *points = new SDL_Point[shape->m_count+1];

		b2Vec2 firstVector;
		bool firstFound = false;
		SDL_Point point;
		// Build list of transformed vertices
		for (int i = 0; i < shape->m_count; ++i) {

			b2Vec2 vector = shape->m_vertices[i];
			//If this is the first vector, then save it
			if (firstFound == false) {
				firstVector = vector;
				firstFound = true;
			}

			point.x = vector.x * Game::config.scaleFactor;
			point.y = vector.y * Game::config.scaleFactor;
			points[i] = point;
		}

		//Add the first point to the end to complete closed shape
		point.x = firstVector.x;
		point.y = firstVector.y;
		points[shape->m_count] = point;

		SDL_RenderDrawLines(this->pRenderer, points, shape->m_count+1);

		delete[] points;

		fixture = body->GetFixtureList()->GetNext();
	}

}

