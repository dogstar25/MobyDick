#include <iostream>
#include <fstream>
#include <math.h>
#include <list>

#include <SDL_ttf.h>

#include "TextureManager.h"
#include "game.h"
#include "WorldObject.h"



bool TextureManager::init(SDL_Window* pWindow)
{

	//Create the main renderer
	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 0);

	//Load all of the textures for the game
	loadTextures();

	return true;
}

void TextureManager::render(WorldObject* gameObject)
{

	SDL_Rect srcRect, destRect;

	//calculate the destination rectangle - must convert meters to pixels with scale factor
	destRect.w = (gameObject->definition->xSize * game->config.scaleFactor);
	destRect.h = (gameObject->definition->ySize * game->config.scaleFactor);
	destRect.x = round((gameObject->physicsBody->GetPosition().x *  game->config.scaleFactor) - (destRect.w / 2));
	destRect.y = round((gameObject->physicsBody->GetPosition().y *  game->config.scaleFactor) - (destRect.h / 2));

	//Get the angle of the object and convert it from Radians to Degrees for SDL
	float angle = gameObject->physicsBody->GetAngle();
	angle = angle * 180 / M_PI;

	//Adjust position based on current camera position - offset
	destRect.x -= game->camera.frame.x;
	destRect.y -= game->camera.frame.y;

	//If this is animated object then get its current animation frame texture, 
	// otherwise get its static texture
	SDL_Texture* texure = NULL;
	SDL_Rect *textureSourceRect = NULL;
	if (gameObject->definition->isAnimated) {

		texure = gameObject->definition->animations[gameObject->currentAnimationState]->texture;
		textureSourceRect = &gameObject->definition->animations[gameObject->currentAnimationState]->currentTextureAnimationSrcRect;
	}
	else {

		texure = gameObject->staticTexture;
	}

	//Render the texture
	SDL_RenderCopyEx(pRenderer, texure, textureSourceRect, &destRect, angle,
		NULL, SDL_FLIP_NONE);

}

void TextureManager::render(PlayerObject* gameObject)
{

	this->render(static_cast<WorldObject*>(gameObject));


}

void TextureManager::render(GameObject* gameObject)
{

	SDL_Rect srcRect, destRect;

	//calculate the destination rectangle - must convert meters to pixels with scale factor
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(gameObject->staticTexture, NULL, NULL, &texW, &texH);

	destRect.w = texW;
	destRect.h = texH;
	destRect.x = gameObject->xPos; //times the map grid size?
	destRect.y = gameObject->xPos; //times the map grid size?

	SDL_Texture* texure = gameObject->staticTexture;
	SDL_Rect *textureSourceRect = NULL;

	//Render the texture
	//SDL_RenderCopy(pRenderer, texure, textureSourceRect, &destRect);
	SDL_RenderCopyEx(pRenderer, texure, textureSourceRect, &destRect, 0,
		NULL, SDL_FLIP_NONE);


}

bool TextureManager::loadTextures()
{

	//Read file and stream it to a JSON object
	Json::Value root;
	ifstream ifs("assets/textureAssets.json");
	//ifstream ifs("assets/textureAssets_Test.json");
	ifs >> root;

	//Get and store config values
	string filename, id;
	int size;
	bool retainSurface = false;

	SDL_Surface* surface;
	SDL_Texture* sdlTexture;
	unique_ptr<Texture> textureObject;
	TTF_Font* fontObject;

	//Loop through every texture defined in the config file, create a texture object
	//and store it in the main texture map
	for(auto itr : root["textures"])
	{
		//textureObject = new Texture();
		textureObject = make_unique<Texture>();

		id = itr["id"].asString();
		filename = itr["filename"].asString();
		retainSurface = itr["retainSurface"].asBool();

		if (itr["textTexture"].isNull() == false)
		{
			SDL_Color color = { 255, 255, 154 };
			size = itr["textTexture"]["size"].asInt();
			TTF_Font* fontObject = TTF_OpenFont(filename.c_str(), size);
			string labelText = itr["textTexture"]["label"].asString();
			surface = TTF_RenderText_Solid(fontObject, labelText.c_str(), color);
			TTF_CloseFont(fontObject);
			string test = TTF_GetError();
		}
		else
		{
			surface = IMG_Load(filename.c_str());
		}

		sdlTexture = SDL_CreateTextureFromSurface(this->pRenderer, surface);
		textureObject->sdlTexture = sdlTexture;
		if (retainSurface == true)
		{
			textureObject->surface = surface;
		}
		else
		{
			SDL_FreeSurface(surface);
		}

		this->textureMap.emplace(id, move(textureObject));
		textureObject.reset();

	}




	return true;
}

const Texture * TextureManager::getTexture(string id)
{
	Texture* textureObject;

	textureObject = this->textureMap[id].get();
	if (textureObject == NULL)
	{
		textureObject = this->textureMap["TX_DEFAULT"].get();
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

			point.x = vector.x * game->config.scaleFactor;
			point.y = vector.y * game->config.scaleFactor;
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

void TextureManager::drawPoints(SDL_Point *points)
{
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
	SDL_RenderDrawLines(this->pRenderer, points, 5);

}

void TextureManager::drawLine(b2Vec2 start, b2Vec2 end)
{
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(pRenderer, start.x, start.y, end.x, end.y);

}

TextureManager::TextureManager()
{

}
TextureManager::~TextureManager()
{

	for (auto&& textureItem : this->textureMap) {
		//pointer->functionOfYourClass();
		if (textureItem.second != NULL) {

			if (textureItem.second->surface != NULL) {
				SDL_FreeSurface(textureItem.second->surface);
			}
			SDL_DestroyTexture(textureItem.second->sdlTexture);

		}
	}

	this->textureMap.clear();
	
}





