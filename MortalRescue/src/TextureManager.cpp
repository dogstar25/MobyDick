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


void TextureManager::render(SDL_Texture* texture, SDL_Color color, SDL_Rect* textureSourceRect, SDL_Rect* destRect, float angle)
{
	//Set the color
	SDL_SetTextureAlphaMod(texture, color.a);

	//SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);

	//Set the render color based on the objects color
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);

	//Render the texture
	SDL_RenderCopyEx(this->pRenderer, texture, textureSourceRect, destRect, angle, NULL, SDL_FLIP_NONE);

}

void TextureManager::render(SDL_Rect* destRect, SDL_Color color)
{

	//Render the rectangle
	SDL_SetRenderDrawColor(this->pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(this->pRenderer, destRect);

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
	//this->textureMap.insert_or_assign(id, make_unique<Texture>(*texture));
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

	int textSize = textObject->definition->textDetails.size; // default to x size
	string fontFile = this->getFont(textObject->fontId);

	TTF_Font* fontObject = TTF_OpenFont(fontFile.c_str(), textSize);
	//surface = TTF_RenderText_Solid(fontObject, textObject->textValue.c_str(), color);
	surface = TTF_RenderText_Blended(fontObject, textObject->textValue.c_str(), color);
	TTF_CloseFont(fontObject);
	string test = TTF_GetError();

	//Set the size of the textObject now that its texture has been generated
	if (surface != NULL)
	{
		textObject->xSize = surface->w;
		textObject->ySize = surface->h;
	}

	sdlTexture = SDL_CreateTextureFromSurface(this->pRenderer, surface);
	SDL_FreeSurface(surface);

	texture->sdlTexture = sdlTexture;

	//Add it to the main texture map
	//Append "TEXTURE" to gameobejct id to create textture id
	//possible memory leak - delete texture before adding in case it is already there
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

	//check the clock and see if enough time as gone by
	steady_clock::time_point now_time = steady_clock::now();
	std::chrono::duration<double> time_diff = now_time - newText->time_snapshot;

	if (newText->hasChanged == true && time_diff.count() > .2)
	{
		//update the timestamp
		newText->time_snapshot = now_time;

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
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

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

void TextureManager::outLineObject(GameObject* gameObject, float lineSize)
{

	vector<SDL_Point> points;
	SDL_Rect gameObjectDrawRect = gameObject->getRenderDestRect();
	float saveScaleX, saveScaleY;
	SDL_Point point;

	//Adjust for camera
	if (gameObject->definition->absolutePositioning == false)
	{
		gameObjectDrawRect.x -= game->camera.frame.x;
		gameObjectDrawRect.y -= game->camera.frame.y;
	}

	//topleft
	point.x = gameObjectDrawRect.x / lineSize;
	point.y = gameObjectDrawRect.y / lineSize;
	points.push_back(point);

	//topright
	point.x = (gameObjectDrawRect.x + gameObjectDrawRect.w) / lineSize;
	point.y = gameObjectDrawRect.y / lineSize;
	points.push_back(point);

	//bottomright
	point.x = (gameObjectDrawRect.x + gameObjectDrawRect.w) / lineSize;
	point.y = (gameObjectDrawRect.y + gameObjectDrawRect.h) / lineSize;
	points.push_back(point);

	//bottomleft
	point.x = gameObjectDrawRect.x / lineSize;
	point.y = (gameObjectDrawRect.y + gameObjectDrawRect.h) / lineSize;
	points.push_back(point);

	//add the topleft as last point to complete the shape
	point.x = gameObjectDrawRect.x / lineSize;
	point.y = gameObjectDrawRect.y / lineSize;
	points.push_back(point);

	//Set render scale to match linesize passed in
	SDL_RenderGetScale(this->pRenderer, &saveScaleX, &saveScaleY);
	SDL_RenderSetScale(this->pRenderer, lineSize, lineSize);
	this->drawPoints(points.data());

	//Rest Scale to whatever is was before
	SDL_RenderSetScale(this->pRenderer, saveScaleX, saveScaleY);

	points.clear();

}

void TextureManager::outLineObject(WorldObject* gameObject)
{

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





