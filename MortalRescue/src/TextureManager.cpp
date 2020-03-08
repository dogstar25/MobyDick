#include <iostream>
#include <fstream>
#include <math.h>
#include <list>

#include <SDL_ttf.h>

#include "TextureManager.h"
#include "Texture.h"
#include "Game.h"
#include "PlayerObject.h"
#include "TextObject.h"
#include "WorldObject.h"
#include "ParticleObject.h"
#include "GameConfig.h"


TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

	for (auto&& textureItem : m_textureMap) {
		//pointer->functionOfYourClass();
		if (textureItem.second != NULL) {

			if (textureItem.second->surface != NULL) {
				SDL_FreeSurface(textureItem.second->surface);
			}
			SDL_DestroyTexture(textureItem.second->sdlTexture);

		}
	}

	m_textureMap.clear();

}


TextureManager& TextureManager::instance()
{
	static TextureManager singletonInstance;
	return singletonInstance;

}

bool TextureManager::init(SDL_Window* pWindow)
{

	//Create the main renderer
	m_Renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);

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
	SDL_RenderCopyEx(m_Renderer, texture, textureSourceRect, destRect, angle, NULL, SDL_FLIP_NONE);

}

void TextureManager::render(SDL_Rect* destRect, SDL_Color color)
{

	//Render the rectangle
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(m_Renderer, destRect);

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

		sdlTexture = SDL_CreateTextureFromSurface(m_Renderer, surface);
		textureObject->sdlTexture = sdlTexture;
		if (retainSurface == true)
		{
			textureObject->surface = surface;
			
		}
		else
		{
			SDL_FreeSurface(surface);
		}

		m_textureMap.emplace(id, move(textureObject));
		textureObject.reset();

	}

	// Loop through every font defined and store it in the main font map
		for (auto itr : root["fonts"])
		{
			id = itr["id"].asString();
			filename = itr["filename"].asString();
			m_fontMap.emplace(id, filename);

		}

	return true;
}

SDL_Texture* TextureManager::createTextureFromSurface(SDL_Surface* surface)
{
	SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(m_Renderer, surface);
	return sdlTexture;
}

string TextureManager::getFont(string id)
{
	string fontFile;

	auto iter = m_fontMap.find(id);

	if (iter != m_fontMap.end())
	{
		//fontFile = m_fontMap[id];
		fontFile = iter->second;
	}
	else //default
	{
		fontFile = m_fontMap["FONT_ARIAL_REG"];
	}

	return fontFile;
}


Texture * TextureManager::getTexture(string id)
{
	Texture* textureObject=NULL;

	auto iter = m_textureMap.find(id);

	if (iter != m_textureMap.end())
	{
		textureObject = iter->second.get();
	}
	else
	{
		textureObject = m_textureMap["TX_DEFAULT"].get();
	}

	return textureObject;
}



bool TextureManager::present()
{
	SDL_RenderPresent(m_Renderer);
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);

	return true;
}

bool TextureManager::clear()
{
	SDL_RenderClear(m_Renderer);

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

			point.x = vector.x * GameConfig::instance().scaleFactor();
			point.y = vector.y * GameConfig::instance().scaleFactor();
			points[i] = point;
		}

		//Add the first point to the end to complete closed shape
		point.x = firstVector.x;
		point.y = firstVector.y;
		points[shape->m_count] = point;

		SDL_RenderDrawLines(m_Renderer, points, shape->m_count+1);

		delete[] points;

		fixture = body->GetFixtureList()->GetNext();
	}

}

void TextureManager::drawPoints(SDL_Point *points)
{

	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
	SDL_RenderDrawLines(m_Renderer, points, 5);

}

void TextureManager::drawLine(b2Vec2 start, b2Vec2 end)
{
	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(m_Renderer, start.x, start.y, end.x, end.y);

}

void TextureManager::outLineObject(GameObject* gameObject, float lineSize)
{

	vector<SDL_Point> points;
	SDL_Rect gameObjectDrawRect = gameObject->getRenderDestRect();
	float saveScaleX, saveScaleY;
	SDL_Point point;

	//Adjust for camera
	if (gameObject->definition()->absolutePositioning == false)
	{
		gameObjectDrawRect.x -= Camera::instance().frame().x;
		gameObjectDrawRect.y -= Camera::instance().frame().y;
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
	SDL_RenderGetScale(m_Renderer, &saveScaleX, &saveScaleY);
	SDL_RenderSetScale(m_Renderer, lineSize, lineSize);
	this->drawPoints(points.data());

	//Rest Scale to whatever is was before
	SDL_RenderSetScale(m_Renderer, saveScaleX, saveScaleY);

	points.clear();

}

void TextureManager::outLineObject(WorldObject* gameObject)
{

}







