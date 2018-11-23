#include "TextureManager.h"

bool TextureManager::init(SDL_Window* pWindow)
{

	//Create the main renderer
	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);

	//Load all of the textures for the game
	loadTextures();

	return true;
}

bool TextureManager::render(GameObject* gameObject)
{
	SDL_Rect srcRect, destRect;

	SDL_RenderClear(pRenderer);
	destRect.x = gameObject->xPos;
	destRect.y = gameObject->yPos;
	destRect.w = gameObject->xSize;
	destRect.h = gameObject->ySize;

	SDL_Texture* tex = gameObject->staticTexture;
	SDL_RenderCopy(pRenderer, tex, NULL, &destRect);
	//SDL_RenderCopyEx - allows for rotation and flipping

	SDL_RenderPresent(pRenderer);

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
	SDL_Surface* tempSurface;
	SDL_Texture* texture;

	//Loop through every texture defined in the config file, create a texture object
	//and store it in the main texture map
	for(auto itr : root["textures"])
	{
		id = itr["id"].asString();
		filename = itr["filename"].asString();

		tempSurface = IMG_Load(filename.c_str());
		texture = SDL_CreateTextureFromSurface(this->pRenderer, tempSurface);
		SDL_FreeSurface(tempSurface);

		//this->textureMap.insert(pair<string, SDL_Texture*>(id, texture));
		this->textureMap[id]= texture;

	}
	

	return true;
}

SDL_Texture * TextureManager::getTexture(string id)
{
	SDL_Texture *tempTexture= nullptr;

	tempTexture = this->textureMap[id];

	return tempTexture;
}


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
	SDL_DestroyRenderer(pRenderer);
	
}
