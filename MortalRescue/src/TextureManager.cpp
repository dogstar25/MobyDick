#include "TextureManager.h"
#include "game.h"

bool TextureManager::init(SDL_Window* pWindow)
{

	//Create the main renderer
	pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);

	//SDL_RenderSetScale(pRenderer, (1/Game::config.scaleFactor), (1/Game::config.scaleFactor));
	//SDL_RenderSetScale(pRenderer, 25 ,25);

	//Load all of the textures for the game
	loadTextures();

	return true;
}

bool TextureManager::render(GameObject* gameObject)
{
	SDL_Rect srcRect, destRect;

	destRect.w = (gameObject->xSize * Game::config.scaleFactor);
	destRect.h = (gameObject->ySize * Game::config.scaleFactor);

	destRect.x = (gameObject->physicsBody->GetPosition().x *  Game::config.scaleFactor) - (destRect.w /2) ;
	destRect.y = (gameObject->physicsBody->GetPosition().y *  Game::config.scaleFactor) - (destRect.h /2) ;
	//destRect.w = gameObject->xSize * Game::config.scaleFactor;
	//destRect.h = gameObject->ySize * Game::config.scaleFactor;

	//If this is a primitive shape object just drawa a rectangle
	if (gameObject->isPrimitiveShape == true)
	{
		SDL_SetRenderDrawColor(pRenderer, 
			gameObject->primativeColor.r, 
			gameObject->primativeColor.g, 
			gameObject->primativeColor.b, 
			gameObject->primativeColor.a);
		SDL_RenderFillRect(pRenderer, &destRect);
	}
	else
	{
		SDL_Texture* tex = gameObject->staticTexture;
		//SDL_RenderCopy(pRenderer, tex, NULL, &destRect);
		SDL_Point *center = new SDL_Point();
		center->x = gameObject->physicsBody->GetPosition().x;
		center->y = gameObject->physicsBody->GetPosition().y;
		SDL_RenderCopyEx(pRenderer, tex, NULL, &destRect, gameObject->physicsBody->GetAngle(),
			center, SDL_FLIP_NONE);


		
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

bool TextureManager::present()
{
	SDL_RenderPresent(pRenderer);
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);

	return true;
}

bool TextureManager::clear()
{
	SDL_RenderClear(pRenderer);

	return true;
}

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
	SDL_DestroyRenderer(pRenderer);
	
}
