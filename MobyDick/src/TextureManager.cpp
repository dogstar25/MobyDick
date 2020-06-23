#include "TextureManager.h"

#include <fstream>

#include <json/json.h>

#include "GameConfig.h"
#include "Camera.h"


TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{

	for (auto&& textureItem : m_textureMap) {

		if (textureItem.second != NULL) {

			if (textureItem.second->surface != NULL) {
				SDL_FreeSurface(textureItem.second->surface);
			}
			SDL_DestroyTexture(textureItem.second->sdlTexture);

		}
	}

	m_textureMap.clear();
	std::map<std::string, std::shared_ptr<Texture>>().swap(m_textureMap);

}


TextureManager& TextureManager::instance()
{
	static TextureManager singletonInstance;
	return singletonInstance;

}

bool TextureManager::init(SDL_Window* window)
{

	//Create the main renderer
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	//Load all of the textures for the game
	loadTextures();

	return true;
}

bool TextureManager::hasTexture(std::string textureId)
{

	if (m_textureMap.find(textureId) == m_textureMap.end())
	{
		return false;
	}
	else
	{
		return true;
	}


}

void TextureManager::addOrReplaceTexture(std::string textureId, std::shared_ptr<Texture> texture)
{
	std::shared_ptr<Texture> tempTexture;

	if (hasTexture(textureId) == false)
	{
		m_textureMap.emplace(textureId, texture);
	}
	else
	{
		tempTexture = getTexture(textureId);

		SDL_FreeSurface(tempTexture->surface);
		if (tempTexture->sdlTexture != NULL) {
			SDL_DestroyTexture(tempTexture->sdlTexture);
		}

		//Use [] for the replacing of an item already there
		m_textureMap[textureId] = texture;
	}

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
	std::ifstream ifs("assets/textureAssets.json");
	//ifstream ifs("assets/textureAssets_Test.json");
	ifs >> root;

	//Get and store config values
	std::string filename, id;
	int size;
	bool retainSurface = false;

	SDL_Surface* surface;
	SDL_Texture* sdlTexture;
	Texture* textureObject;
	TTF_Font* fontObject;

	//Loop through every texture defined in the config file, create a texture object
	//and store it in the main texture map
	for(auto itr : root["textures"])
	{
		//textureObject = new Texture();
		textureObject = new Texture();

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

		m_textureMap.emplace(id, std::make_shared<Texture>(*textureObject));

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

std::string TextureManager::getFont(std::string id)
{
	std::string fontFile;

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


std::shared_ptr<Texture> TextureManager::getTexture(std::string id)
{
	std::shared_ptr<Texture> textureObject;

	auto iter = m_textureMap.find(id);

	if (iter != m_textureMap.end())
	{
		textureObject = iter->second;
	}
	else
	{
		textureObject = m_textureMap["TX_DEFAULT"];
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

void TextureManager::drawGlowLine(b2Vec2 start, b2Vec2 end, SDL_Color color)
{
	SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(m_Renderer, start.x, start.y, end.x, end.y);
	SDL_RenderDrawLine(m_Renderer, start.x, start.y, end.x, end.y);
	SDL_RenderDrawLine(m_Renderer, start.x+1, start.y-1, end.x+1, end.y-1);
	SDL_RenderDrawLine(m_Renderer, start.x -1, start.y+1, end.x -1, end.y+1);
	SDL_RenderDrawLine(m_Renderer, start.x + 2, start.y - 2, end.x + 2, end.y - 2);
	SDL_RenderDrawLine(m_Renderer, start.x - 2, start.y + 2, end.x - 2, end.y + 2);




}

void TextureManager::drawGlowLine2(b2Vec2 start, b2Vec2 end, SDL_Color color)
{
//	SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_ADD);
	SDL_Texture* texture=NULL;
	texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1280, 720);

	SDL_SetRenderTarget(m_Renderer, texture);
	SDL_RenderSetScale(m_Renderer, 7, 7);
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	
	SDL_RenderDrawLine(m_Renderer, start.x, start.y, end.x, end.y);
	SDL_RenderCopy(m_Renderer, texture, NULL, NULL);

	SDL_SetRenderTarget(m_Renderer, NULL);
	SDL_RenderSetScale(m_Renderer, 1, 1);

}

void TextureManager::outlineObject(std::vector<SDL_Point> points, float lineSize)
{

	float saveScaleX, saveScaleY;

	//Set render scale to match linesize passed in
	SDL_RenderGetScale(m_Renderer, &saveScaleX, &saveScaleY);
	SDL_RenderSetScale(m_Renderer, lineSize, lineSize);
	this->drawPoints(points.data());

	//Rest Scale to whatever is was before
	SDL_RenderSetScale(m_Renderer, saveScaleX, saveScaleY);


}








