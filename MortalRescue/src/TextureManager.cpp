#include <iostream>
#include <fstream>
#include <math.h>
#include <list>

#include <SDL_ttf.h>

#include "TextureManager.h"
#include "game.h"
#include "PlayerObject.h"
#include "TextObject.h"
#include "WorldObject.h"
#include "ParticleObject.h"



bool TextureManager::init(SDL_Window* pWindow)
{

	//Create the main renderer
	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 0);

	//Load all of the textures for the game
	loadTextures();

	return true;
}


void TextureManager::getRenderDestRect(GameObject* gameObject, SDL_Rect* destRect)
{

	destRect->w = gameObject->xSize;
	destRect->h = gameObject->xSize;
	destRect->x = gameObject->xPos;
	destRect->y = gameObject->yPos;

	return;

}

/*
void TextureManager::getRenderDestRect(WorldObject* gameObject, SDL_Rect* destRect)
{

	destRect->w = (gameObject->xSize * game->config.scaleFactor);
	destRect->h = (gameObject->ySize * game->config.scaleFactor);
	destRect->x = round((gameObject->physicsBody->GetPosition().x *  game->config.scaleFactor) - (destRect->w / 2));
	destRect->y = round((gameObject->physicsBody->GetPosition().y *  game->config.scaleFactor) - (destRect->h / 2));

	//Adjust position based on current camera position - offset
	destRect->x -= game->camera.frame.x;
	destRect->y -= game->camera.frame.y;


	return;

}
*/
/*
void TextureManager::getRenderDestRect(TextObject* gameObject, SDL_Rect* destRect)
{

	//calculate the destination rectangle - must convert meters to pixels with scale factor
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(gameObject->texture->sdlTexture, NULL, NULL, &texW, &texH);

	destRect->w = texW;
	destRect->h = texH;
	destRect->x = gameObject->xPos;
	destRect->y = gameObject->yPos;

	return;

}
*/
/*
void TextureManager::getRenderDestRect(PlayerObject* gameObject, SDL_Rect* destRect)
{

	//Use same as World Object
	this->getRenderDestRect(static_cast<WorldObject*>(gameObject), destRect);
	

}

void TextureManager::getRenderDestRect(ParticleObject* gameObject, SDL_Rect* destRect)
{

	//Use same as World Object
	this->getRenderDestRect(static_cast<WorldObject*>(gameObject), destRect);


}
*/
/*
void TextureManager::getRenderTexture(GameObject* gameObject, SDL_Texture* texture, SDL_Rect* textureSrcRect)
{

	textureSrcRect = NULL;
	if (gameObject->isAnimated) {

		texture = gameObject->animations[gameObject->currentAnimationState]->texture;
		textureSrcRect = &gameObject->animations[gameObject->currentAnimationState]->currentTextureAnimationSrcRect;
	}
	else {

		texture = gameObject->texture->sdlTexture;
	}


}
*/

/*
void TextureManager::getRenderTexture(WorldObject* gameObject, SDL_Texture* texture, SDL_Rect* textureSrcRect)
{

	//Use same as Game Object
	this->getRenderTexture(static_cast<GameObject*>(gameObject), texture, textureSrcRect);


}

void TextureManager::getRenderTexture(PlayerObject* gameObject, SDL_Texture* texture, SDL_Rect* textureSrcRect)
{

	//Use same as Game Object
	this->getRenderTexture(static_cast<GameObject*>(gameObject), texture, textureSrcRect);


}
*/

void TextureManager::renderTexture(SDL_Texture* texture, SDL_Rect* textureSourceRect, SDL_Rect* destRect, float angle)
{

	//Render the texture
	SDL_RenderCopyEx(this->pRenderer, texture, textureSourceRect, destRect, angle, NULL, SDL_FLIP_NONE);

}


/*
void TextureManager::render(GameObject* gameObject)
{

	SDL_Rect srcRect, destRect;

	//Get value of destination rectangle for render
	getRenderDestRect(gameObject, &destRect);

	SDL_Texture* texture = gameObject->texture->sdlTexture;
	SDL_Rect *textureSourceRect;
	getRenderTexture(gameObject, texture, textureSourceRect);

	//Render the texture
	SDL_RenderCopyEx(pRenderer, texture, textureSourceRect, &destRect, 0,
		NULL, SDL_FLIP_NONE);

}
*/

/*
void TextureManager::render(WorldObject* gameObject)
{

	SDL_Rect srcRect, destRect;

	//Get value of destination rectangle for render
	getRenderDestRect(gameObject, &destRect);

	//Get the angle of the object and convert it from Radians to Degrees for SDL
	float angle = gameObject->physicsBody->GetAngle();
	angle = angle * 180 / M_PI;

	//Get the textture and if its animated, the texture source rectanle
	SDL_Texture* texture = gameObject->texture->sdlTexture;
	SDL_Rect *textureSourceRect;
	getRenderTexture(gameObject, texture, textureSourceRect);

	//Render the texture
	SDL_RenderCopyEx(pRenderer, texture, textureSourceRect, &destRect, angle,
		NULL, SDL_FLIP_NONE);


	//Need support for a primitive worldobject, or maybe game object, that simply fills a rectangle with a color
	// to be used for UI Panels
	//SDL_RenderFillRect

}
*/
/*
void TextureManager::render(PlayerObject* gameObject)
{

	SDL_Rect srcRect, destRect;

	//Get value of destination rectangle for render
	getRenderDestRect(gameObject, &destRect);

	//Get the angle of the object and convert it from Radians to Degrees for SDL
	float angle = gameObject->physicsBody->GetAngle();
	angle = angle * 180 / M_PI;

	//If this is animated object then get its current animation frame texture, 
	// otherwise get its static texture
	SDL_Texture* texture = NULL;
	SDL_Rect *textureSourceRect = NULL;
	if (gameObject->isAnimated) {

		texture = gameObject->animations[gameObject->currentAnimationState]->texture;
		textureSourceRect = &gameObject->animations[gameObject->currentAnimationState]->currentTextureAnimationSrcRect;
	}
	else {

		texture = gameObject->texture->sdlTexture;
	}


	//test. copy the texture to new object first so we can muck with it without affecting other obejcts sharing the same texture

	//SDL_SetTextureAlphaMod(texture, randomInt); //seems helpful for particles

	//SDL_GetTextureColorMod(texture, &r, &g, &b);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD); //alpha ADD seems good for particles
	//SDL_RenderSetScale(pRenderer, randomInt, randomInt); // nope


	if (gameObject->definitionId == "PARTICLE_SMOKE_1") {

		int randomR = game->util.generateRandomNumber(204, 224);
		int randomG = game->util.generateRandomNumber(8, 89);
		int randomB = game->util.generateRandomNumber(8, 11);
		SDL_SetTextureColorMod(texture, randomR, randomG, randomB); // doesnt seem helpful for particles
	}


	//Render the texture
	SDL_RenderCopyEx(pRenderer, texture, textureSourceRect, &destRect, angle,
		NULL, SDL_FLIP_NONE);


	//Need support for a primitive worldobject, or maybe game object, that simply fills a rectangle with a color
	// to be used for UI Panels
	//SDL_RenderFillRect
	//this->render(static_cast<WorldObject*>(gameObject));


}
*/

/*
void TextureManager::render(ParticleObject* gameObject)
{

	SDL_Rect srcRect, destRect;

	//Get value of destination rectangle for render
	getRenderDestRect(gameObject, &destRect);

	//Get the angle of the object and convert it from Radians to Degrees for SDL
	float angle = gameObject->physicsBody->GetAngle();
	angle = angle * 180 / M_PI;

	//If this is animated object then get its current animation frame texture, 
	// otherwise get its static texture
	SDL_Texture* texture = NULL;
	SDL_Rect *textureSourceRect = NULL;
	if (gameObject->isAnimated) {

		texture = gameObject->animations[gameObject->currentAnimationState]->texture;
		textureSourceRect = &gameObject->animations[gameObject->currentAnimationState]->currentTextureAnimationSrcRect;
	}
	else {

		texture = gameObject->texture->sdlTexture;
	}


	//test. copy the texture to new object first so we can muck with it without affecting other obejcts sharing the same texture

	//SDL_SetTextureAlphaMod(texture, randomInt); //seems helpful for particles

	//SDL_GetTextureColorMod(texture, &r, &g, &b);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD); //alpha ADD seems good for particles
	//SDL_RenderSetScale(pRenderer, randomInt, randomInt); // nope


	if (gameObject->definitionId == "PARTICLE_SMOKE_1") {

		int randomR = game->util.generateRandomNumber(204, 224);
		int randomG = game->util.generateRandomNumber(8, 89);
		int randomB = game->util.generateRandomNumber(8, 11);
		SDL_SetTextureColorMod(texture, randomR, randomG, randomB); // doesnt seem helpful for particles
	}


	//Render the texture
	SDL_RenderCopyEx(pRenderer, texture, textureSourceRect, &destRect, angle,
		NULL, SDL_FLIP_NONE);


	//Need support for a primitive worldobject, or maybe game object, that simply fills a rectangle with a color
	// to be used for UI Panels
	//SDL_RenderFillRect
	//this->render(static_cast<WorldObject*>(gameObject));


}
*/
/*
void TextureManager::render(TextObject* gameObject)
{

	SDL_Rect srcRect, destRect;

	//Get value of destination rectangle for render
	getRenderDestRect(gameObject, &destRect);

	SDL_Texture* texure = gameObject->texture->sdlTexture;
	SDL_Rect *textureSourceRect = NULL;

	//Render the texture
	//SDL_RenderCopy(pRenderer, texure, textureSourceRect, &destRect);
	SDL_RenderCopyEx(pRenderer, texure, textureSourceRect, &destRect, 0,
		NULL, SDL_FLIP_NONE);

}
*/


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

		surface = IMG_Load(filename.c_str());

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

	// Loop through every font defined and store it in the main font map
		for (auto itr : root["fonts"])
		{
			id = itr["id"].asString();
			filename = itr["filename"].asString();
			this->fontMap.emplace(id, filename);

		}

	return true;
}

void TextureManager::addTexture(string id, Texture* texture)
{

	//this->gameObjects[layer].push_back(make_unique<TextObject>(*gameObject));

	//memory leak here
	//this->textureMap.erase(id);
	this->textureMap.insert_or_assign(id, make_unique<Texture>(*texture));
	//this->textureMap.emplace(id, make_unique<Texture>(*texture));


}



Texture* TextureManager::generateTextTexture(TextObject* textObject)
{

	SDL_Surface* surface;
	Texture* texture = new Texture();
	SDL_Texture* sdlTexture;

	SDL_Color color = { textObject->color.r, 
		textObject->color.g,
		textObject->color.b,
		textObject->color.a };

	int textSize = textObject->xSize; // default to x size
	string fontFile = this->getFont(textObject->fontId);

	TTF_Font* fontObject = TTF_OpenFont(fontFile.c_str(), textSize);
	surface = TTF_RenderText_Solid(fontObject, textObject->textValue.c_str(), color);
	TTF_CloseFont(fontObject);
	string test = TTF_GetError();

	sdlTexture = SDL_CreateTextureFromSurface(this->pRenderer, surface);
	SDL_FreeSurface(surface);

	texture->sdlTexture = sdlTexture;

	//Add it to the main texture map
	//Append "TEXTURE" to gameobejct id to create textture id
	string textureId = textObject->id + "_TEXT_TEXTURE";
	this->addTexture(textureId, texture);
	return 	texture;
	
}


Texture* TextureManager::updateDynamicTextTexture(TextObject *textObject)
{

	textItem* newText;
	Texture* textureObject;
	SDL_Surface* surface;

	//newText = game->dynamicTextManager.textItems[gameObject->definition->id].get();
	newText = game->dynamicTextManager.getTextItem(textObject->definitionId);

	if (newText->hasChanged == true)
	{

		//Destroy this texture from the map before we generate a new one - memory leak otherwise
		SDL_DestroyTexture(textObject->texture->sdlTexture);

		//Build new texture
		textObject->textValue = newText->text;
		textureObject = generateTextTexture(textObject);
		newText->hasChanged = false;

	}
	else
	{
		textureObject = textObject->texture;

		//TODO:set a flag at game object level so that the TextureManager::render(TextObject* gameObject) doesnt have to 
		// do SDL_QueryTexture
	}
		
	return textureObject;

}

string TextureManager::getFont(string id)
{
	string fontFile;

	auto iter = this->fontMap.find(id);

	if (iter != this->fontMap.end())
	{
		//fontFile = this->fontMap[id];
		fontFile = iter->second;
	}
	else //default
	{
		fontFile = this->fontMap["FONT_ARIAL_REG"];
	}

	return fontFile;
}


Texture * TextureManager::getTexture(string id)
{
	Texture* textureObject=NULL;

	auto iter = this->textureMap.find(id);

	if (iter != this->textureMap.end())
	{
		textureObject = iter->second.get();
	}
	else
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





